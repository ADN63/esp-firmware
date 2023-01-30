#pragma once
#ifndef _API_PARTIAL_BASE_H
#define _API_PARTIAL_BASE_H

//14

char header_connection[11];
P(p_header_connection) = "Connection";

char header_close[6];
P(p_header_close) = "close";

char header_accept[8];
P(p_header_accept) = "Accept";

char header_location[10];
P(p_header_location) = "Location";

char header_contentType[14];
P(p_header_contentType) = "Content-Type";

char header_contentType_value_json[18];
P(p_header_contentType_value_json) = "application/json";

char header_value_all[2];
P(p_header_value_all) = "*";

char header_origin[30];
P(p_header_origin) = "Access-Control-Allow-Origin";

char header_method[30];
P(p_header_method) = "Access-Control-Allow-Methods";

char header_header[30];
P(p_header_header) = "Access-Control-Allow-Headers";

char api_features[10];
P(p_api_features) = "features";

char api_auth[6];
P(p_api_auth) = "auth";

char api_wifi[6];
P(p_api_wifi) = "wifi";

char api_signin[7];
P(p_api_signin) = "signin";

char api_ota[4];
P(p_api_ota) = "ota";

char api_system[8];
P(p_api_system)= "system";
#endif