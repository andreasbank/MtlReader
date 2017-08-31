/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#ifndef MTLMAP_HPP
#define MTLMAP_HPP

#include <iostream>
#include <string>

enum MtlOptionImfChan { r, g, b, m, l, z };

class MtlMap {
public:
  MtlMap(const std::string& mapName = std::string());
  void printProperties(int tableLevel = 0, bool isLast = false);

  /**
   * Map name
   */
  std::string name;

  /**
   The -blendu option turns texture blending in the horizontal direction
   (u direction) on or off.  The default is on.
   */
  bool blendU; // blendU (On|Off)

  /**
   The -blendv option turns texture blending in the vertical direction (v
   direction) on or off.  The default is on.
   */
  bool blendV; // blendV (On|Off)

  /**
   The -clamp option turns clamping on or off.  When clamping is on,
   textures are restricted to 0-1 in the uvw range.  The default is off.
   */
  bool clamp; // clamp (On|Off)

  /**
   The -imfchan option specifies the channel used to create a scalar or
   bump texture.  Scalar textures are applied to:

   transparency
   specular exponent
   decal
   displacement

   The channel choices are:

   r specifies the red channel.
   g specifies the green channel.
   b specifies the blue channel.
   m specifies the matte channel.
   l specifies the luminance channel.
   z specifies the z-depth channel.

   The default for bump and scalar textures is "l" (luminance), unless you
   are building a decal.  In that case, the default is "m" (matte).
   */
  MtlOptionImfChan imfChan; // imfchan (r|g|b|m|l|z)

  /**
   The -mm option modifies the range over which scalar or color texture
   values may vary.  This has an effect only during rendering and does not
   change the file.

   "base" adds a base value to the texture values.  A positive value makes
   everything brighter; a negative value makes everything dimmer.  The
   default is 0; the range is unlimited.

   "gain" expands the range of the texture values.  Increasing the number
   increases the contrast.  The default is 1; the range is unlimited.
   */
  float mm[2]; // mm (base gain)

  /**
   The -o option offsets the position of the texture map on the surface by
   shifting the position of the map origin.  The default is 0, 0, 0.

   "u" is the value for the horizontal direction of the texture

   "v" is an optional argument.
   "v" is the value for the vertical direction of the texture.

   "w" is an optional argument.
   "w" is the value used for the depth of a 3D texture.
   */
  float offset[3]; // o (u v w)

  /**
   The -s option scales the size of the texture pattern on the textured
   surface by expanding or shrinking the pattern.  The default is 1, 1, 1.

   "u" is the value for the horizontal direction of the texture

   "v" is an optional argument.
   "v" is the value for the vertical direction of the texture.

   "w" is an optional argument.
   "w" is a value used for the depth of a 3D texture.
   "w" is a value used for the amount of tessellation of the displacement
   map.
   */
  float scale[3]; // s (u v w)

  /**
   The -t option turns on turbulence for textures.  Adding turbulence to a
   texture along a specified direction adds variance to the original image
   and allows a simple image to be repeated over a larger area without
   noticeable tiling effects.

   turbulence also lets you use a 2D image as if it were a solid texture,
   similar to 3D procedural textures like marble and granite.

   "u" is the value for the horizontal direction of the texture
   turbulence.

   "v" is an optional argument.
   "v" is the value for the vertical direction of the texture turbulence.

   "w" is an optional argument.
   "w" is a value used for the depth of the texture turbulence.

   By default, the turbulence for every texture map used in a material is
   uvw = (0,0,0).  This means that no turbulence will be applied and the 2D
   texture will behave normally.
   */
  float turbulence[3]; // t (u v w)

  /**
   The -texres option specifies the resolution of texture created when an
   image is used.  The default texture size is the largest power of two
   that does not exceed the original image size.

    If the source image is an exact power of 2, the texture cannot be built
   any larger.  If the source image size is not an exact power of 2, you
   can specify that the texture be built at the next power of 2 greater
   than the source image size.

    The original image should be square, otherwise, it will be scaled to
   fit the closest square size that is not larger than the original.
   Scaling reduces sharpness.
   */
  int textureResolution[2]; // texres (string + "x" + string)

  /** The filename of the image to use for this map */
  std::string fileName;
};

#endif /* MTLMAP_HPP */
