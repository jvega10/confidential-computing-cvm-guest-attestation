#include <iostream>
#include <stdarg.h>
#include <vector>
#include <AttestationClient.h>
#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <nlohmann/json.hpp>
#include "src/Logger.h"

using json = nlohmann::json;
using namespace std;
using namespace std::chrono;

static char *optarg = nullptr;
static int optind = 1;
static int getopt(int argc, char *const argv[], const char *optstring) {
  // Error and -1 returns are the same as for getopt(), plus '?'
  //  for an ambiguous match or an extraneous parameter.
  if (
      (argv == nullptr) ||
      (optind >= argc) ||
      (argv[optind][0] != '-') ||
      (argv[optind][0] == 0)) {
    return -1;
  }

  int opt = argv[optind][1];
  const char *p = strchr(optstring, opt);

  if (p == NULL) {
    return '?';
  }
  if (p[1] == ':') {
    optind++;
    if (optind >= argc) {
      return '?';
    }
    optarg = argv[optind];
    optind++;
  }
  return opt;
}

void usage(char *programName) {
  printf("Usage: %s [arguments]\n", programName);
  printf("   Options:\n");
  printf("\t-c Config file\n");
  printf("\t-h Print this help menu\n\n");
  printf("   Examples:\n");
  printf("\t%s -c config.json\n", programName);
  printf("\t%s -h\n", programName);
}

int main(int argc, char *argv[]) {
  std::string config_filename;
  std::string claims_filename;

  int opt;
  while ((opt = getopt(argc, argv, "c:h")) != -1) {
    switch (opt) {
    case 'c':
      config_filename.assign(optarg);
      break;
    case 'h':
      usage(argv[0]);
      exit(0);
    case ':':
      fprintf(stderr, "Option needs a value\n");
      exit(1);
    default:
      usage(argv[0]);
      exit(1);
    }
  }

  try {
    // user must provide a config file
    if (config_filename.empty()) {
      fprintf(stderr, "Config file is missing\n");
      usage(argv[0]);
      exit(1);
    }

    // set attestation request based on config file
    std::ifstream config_file(config_filename);
    json config = json::parse(config_file);
    config_file.close();

    std::string attestation_url;
    if (!config.contains("attestation_url")) {
      fprintf(stderr, "attestation_url is missing\n\n");
      usage(argv[0]);
      exit(1);
    }
    attestation_url = config["attestation_url"];

    std::string api_key;
    if (config.contains("api_key")) {
      api_key = config["api_key"];
    }

    bool metrics_enabled = false;
    if (config.contains("enable_metrics")) {
      metrics_enabled = config["enable_metrics"];
    }

    std::string provider;
    if (!config.contains("attestation_provider")) {
      fprintf(stderr, "attestation_provider is missing\n\n");
      usage(argv[0]);
      exit(1);
    }
    provider = config["attestation_provider"];

    if ((provider.compare("amber") != 0) && (provider.compare("maa") != 0)) {
      fprintf(stderr, "Attestation provider was incorrect\n\n");
      usage(argv[0]);
      exit(1);
    }

      // check for user claims
      std::string client_payload;
      json user_claims = config["claims"];
      if (!user_claims.is_null()) {
        client_payload = user_claims.dump();
      }

      // if attesting with Amber, we need to make sure an API token was provided
      if (api_key.empty() && (provider.compare("amber") == 0)) {
        fprintf(stderr, "Attestation endpoint \"api_key\" value missing\n\n");
        usage(argv[0]);
        exit(1);
      }

      std::string output_filename;
      if (config.contains("output_filename")) {
        output_filename = config["output_filename"];
      }

      AttestationClient *attestation_client = nullptr;
      Logger *log_handle = new Logger();

      // Initialize attestation client
      if (!Initialize(log_handle, &attestation_client)) {
        fprintf(stderr, "Failed to create attestation client object\n\n");
        Uninitialize();
        exit(1);
      }
      attest::AttestationResult result;

      attest::ClientParameters params = {};
      params.attestation_endpoint_url = (unsigned char *)attestation_url.data();
      params.attestation_provider = (unsigned char *)provider.data();
      params.attestation_api_key = (unsigned char *)api_key.data();
      params.client_payload = (unsigned char *)client_payload.data();

      bool has_token = true;
      auto start = high_resolution_clock::now();

      std::string jwt_token;
      result = attestation_client->AttestTdx(params, jwt_token);

      auto stop = high_resolution_clock::now();
      duration<double, std::milli> elapsed = stop - start;

      if (result.code_ != attest::AttestationResult::ErrorCode::SUCCESS) {
        has_token = false;
      }

      if (has_token) {
        if (jwt_token.empty()) {
          fprintf(stderr, "Empty token received\n");
          Uninitialize();
          exit(1);
        }

        cout << "Hardware attestation passed successfully!!" << endl;
        cout << "TOKEN:\n"
             << endl;
        std::cout << jwt_token << std::endl
                  << std::endl;

        if (metrics_enabled) {
          stringstream stream;
          stream << "Run Summary:\n"
                 << "\tAttestation Time(ms): " << std::to_string(elapsed.count()) << "\n";

          cout << stream.str() << endl;
        }
      }

      Uninitialize();
    }
  catch (std::exception &e) {
    cout << "Exception occured. Details - " << e.what() << endl;
    exit(1);
  }
}