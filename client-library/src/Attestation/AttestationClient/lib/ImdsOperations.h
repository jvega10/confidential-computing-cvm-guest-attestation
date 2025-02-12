//-------------------------------------------------------------------------------------------------
// <copyright file="ImdsOperations.h" company="Microsoft Corporation">
// Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//-------------------------------------------------------------------------------------------------

#pragma once

#include <curl/curl.h>
#include <json/json.h>
#include <chrono>
#include <thread>
#include <math.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "Logging.h"
#include <stdio.h>
#include "AttestationLibTypes.h"
#include "AttestationParameters.h"
#include "Tpm.h"
#include "AttestationClient.h"

class ImdsOperations {
public:

    /**
     * @brief This function will be used to retrieve the VCek Cert from IMDS
     * @param[out] vcek_cert base64 encoded certificate chain
     * @return In case of success, AttestationResult object with error code
     * ErrorCode::Success will be returned.
     * In case of failure, an appropriate ErrorCode will be set in the
     * AttestationResult object and error description will be provided.
     */
    attest::AttestationResult GetVCekCert(std::string& vcek_cert);

    /**
     * @brief This function will be used to retrieve the Platform Evidence from IMDS endpoint.
     * @param[in] imds_request the report data in the form of a JSON string.
     * Ex. {"Report": "<Base64 Endoded Report>"}
     * @param[out] imds_response the platform evidence in the form of a JSON string.
     * Tdx Exmaple: {"Quote": "<Base64 Encoded Quote>"}
     * @return In case of success, AttestationResult object with error code
     * ErrorCode::Success will be returned.
     * In case of failure, an appropriate ErrorCode will be set in the
     * AttestationResult object and error description will be provided.
     */
    attest::AttestationResult GetPlatformEvidence(const std::string &imds_request, std::string &imds_response);
};