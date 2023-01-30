#pragma once
#ifndef _PARTIAL_BASE_H
#define _PARTIAL_BASE_H

#define METHOD awot::Request

enum HTTP_CODE
{
    HTTP_SUCCESS = 200,
    HTTP_REDIR_PERM = 301,
    HTTP_REDIR_TEMP = 302,
    HTTP_BAD_REQ = 400,
    HTTP_NO_AUTH = 401,
    HTTP_ACCES_RESTRICTED = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_ERR_500 = 500,
    HTTP_ERR_502 = 502,
    HTTP_ERR_503 = 503,
    HTTP_ERR_504 = 504,
    JSON_FAILED_DS = 999,
};

enum MyWiFiType
{
    UNKNOW = -1,
    STA = 0,
    AP = 1
};

#include "defs/JsonDefs.h"
#include "defs/ApiDefs.h"
#include "defs/ConfigDefs.h"
#endif