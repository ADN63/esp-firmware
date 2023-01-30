#pragma once
#ifndef _PARTIAL_FEATURE_BASE_H
#define _PARTIAL_FEATURE_BASE_H

class Feature
{
private:
public:
    char name[20]; bool active;
    Feature();
    Feature(const char *name, bool active);
};
Feature::Feature() {}
Feature::Feature(const char *name, bool active)
{
    sprintf_P(this->name,PSTR("%S"), name);
    this->active = active;
}

#endif