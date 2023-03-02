#include "volume_converter.hpp"

namespace common {
    double VolumeConverter::base2quote(double base, double base_volume, double quote) const {
        double coeff = quote / base;
        return base_volume * coeff;
    }
    double VolumeConverter::quote2base(double quote, double quote_volume, double base) const {
        double coeff = base / quote;
        return quote_volume * coeff;
    }
}