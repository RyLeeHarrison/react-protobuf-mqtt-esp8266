#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "Potentiometer.pb.h"

typedef Potentiometer_Brightness Brightness_t;

class Message
{
public:
  static void encode(Brightness_t *data, uint8_t **buffer, size_t *size);
  static bool decode(Brightness_t *data, const uint8_t *payload, size_t size);
};
