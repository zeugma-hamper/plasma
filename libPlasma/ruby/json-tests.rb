# -*- coding: utf-8 -*-

require_relative './test-helpers.rb'
require 'Pool'
require 'rubygems'
require 'json'

include Plasma

KATE_EXAMPLE = <<END
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
--- !protein
descrips:
- key: value
- !cons
  key: value
- key: value
END

KATE_ROUNDTRIP = <<END
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
--- !protein
descrips:
- !!omap
  - key: value
- !cons
  key: value
- !!omap
  - key: value
ingests: !!omap []
...
END

PATRICK_EXAMPLE = <<END
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
--- !protein
descrips:
- dev-settings
ingests:
  13604:
    Type: DxCore160
    DisplayType: MX T160
    Category: MxCam
    FirmwareVersion: 219
    Enabled: true
    StrobeIntensity: 0.80000000000000004
    GreyscaleMode: EGreyscaleNone
    Gain: 1
    Circularity: 0.20000000000000001
    MonitorMode: false
    Threshold: 0.69999999999999996
    MaxPixelsPerLine: 50
    CentroidTrackingEnabled: ENotSupported
    BlobDetectionEnabled: ENotSupported
    Temperature:
    - 40
    - ENormal
    SensorType: AM63
    SensorDimensions: !vector [4704, 3456]
    StrobeType: NIR 56
    PixelAspectRatio: 1.0
    LEDs:
      blue: false
      logo: false
    SupportedThresholdMaps:
      EMk6ThresholdMapType1Bit: !vector [294, 108, 16, 32]
    ActiveThresholdMap: EMk6ThresholdMapType1Bit
  33569154:
    Type: Giganet
    DisplayType: MX Giganet
    Category: MxUltraNet
    FirmwareVersion: 219
    Enabled: true
...
END

PATRICK_ROUNDTRIP = <<END
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
--- !protein
descrips:
- dev-settings
ingests: !!omap
- '13604': !!omap
  - FirmwareVersion: !i64 219
  - Threshold: !f64 0.69999999999999996
  - MaxPixelsPerLine: !i64 50
  - Category: MxCam
  - Enabled: true
  - LEDs: !!omap
    - blue: false
    - logo: false
  - ActiveThresholdMap: EMk6ThresholdMapType1Bit
  - PixelAspectRatio: !f64 1
  - SupportedThresholdMaps: !!omap
    - EMk6ThresholdMapType1Bit: !vector [!i64 294, !i64 108, !i64 16, !i64 32]
  - GreyscaleMode: EGreyscaleNone
  - CentroidTrackingEnabled: ENotSupported
  - StrobeType: NIR 56
  - StrobeIntensity: !f64 0.80000000000000004
  - MonitorMode: false
  - BlobDetectionEnabled: ENotSupported
  - SensorDimensions: !vector [!i64 4704, !i64 3456]
  - Type: DxCore160
  - Circularity: !f64 0.20000000000000001
  - Gain: !i64 1
  - Temperature:
    - !i64 40
    - ENormal
  - SensorType: AM63
  - DisplayType: MX T160
- '33569154': !!omap
  - FirmwareVersion: !i64 219
  - Category: MxUltraNet
  - Enabled: true
  - Type: Giganet
  - DisplayType: MX Giganet
...
END

JSON.create_id = "¿json class?"

class SlawTest < Test::Unit::TestCase

  def test_cons
    j = Slaw.from_yaml(KATE_EXAMPLE).to_json
    y = JSON.parse(j, {:create_additions => true}).to_s
    # KATE_EXAMPLE doesn't contain any maps of length > 1, so it's
    # safe to assume we'll get exactly what we think.
    assert_equal(KATE_ROUNDTRIP, y)
  end

  def test_vect
    j = Slaw.from_yaml(PATRICK_EXAMPLE).to_json
    y = JSON.parse(j, {:create_additions => true}).to_s
    # PATRICK_EXAMPLE contains maps with more than one key, so
    # it's possible they could be permuted (based on whatever
    # hash function Ruby is using for its hashes, which might
    # be different between releases; I don't know, but it could),
    # so I sort the lines in the YAML, which will remove
    # any key-ordering effects.
    assert_equal(PATRICK_ROUNDTRIP.split("\n").sort, y.split("\n").sort)
  end

  def test_rude
    num = 123456789012345678901234567890123456789012345678901234567890
    rude = [num].pack("w")
    p1 = Protein.new(["ugh"], {"what" => "ever"}, rude)
    j = p1.to_json
    p2 = JSON.parse(j, {:create_additions => true})
    assert_equal(num, p2.rude_data.unpack("w")[0])
    assert_equal(p1, p2)
  end

end
