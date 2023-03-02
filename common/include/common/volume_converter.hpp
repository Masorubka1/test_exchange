#ifndef VOLUME_CONVERTER_HPP
#define VOLUME_CONVERTER_HPP

namespace common {
    // add virtual base class for SWAP + FUTURES + SPOT
    class VolumeConverter {
        VolumeConverter() = default;
        double base2quote(double base, double base_volume, double quote) const;
        double quote2base(double quote, double quote_volume, double base) const;
    };
}

#endif