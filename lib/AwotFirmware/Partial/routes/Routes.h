#pragma once
#ifndef _ROUTES_BASE_H
#define _ROUTES_BASE_H

char route_api_config[13];
P(p_route_api_config) = "/api/config";

char route_api_wifi[13];
P(p_route_api_wifi) = "/api/wifi";

char route_api_ota[10];
P(p_route_api_ota) = "/api/ota";

char route_api_feature[15];
P(p_route_api_feature) = "/api/features";

char route_api_auth[11];
P(p_route_api_auth) = "/api/auth";

char route_api_auth_signin[20];
P(p_route_api_auth_signin) = "/api/auth/signin";

char route_api_auth_signout[20];
P(p_route_api_auth_signout) = "/api/auth/signout";

char route_api_generate_204[20];
P(p_route_api_generate_204) = "/generate_204";

char route_api_networks[19];
P(p_route_api_networks) = "/api/network-scan";

char route_api_factory_reset[20];
P(p_route_api_factory_reset) = "/api/factory-reset";

char route_api_system[13];
P(p_route_api_system) = "/api/system";

#endif