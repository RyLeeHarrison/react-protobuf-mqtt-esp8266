#include "Message.h"

bool Message::decode(Brightness_t *data, const uint8_t *payload, size_t length) {
  pb_istream_t stream = pb_istream_from_buffer(payload, length);
  
  if (!pb_decode(&stream, Potentiometer_Brightness_fields, data)) {
    return false;
  }
  
  return true;
}

void Message::encode(Brightness_t *data, uint8_t **buffer, size_t *length)
{
  uint8_t message[200];

  pb_ostream_t sendStream = pb_ostream_from_buffer(message, sizeof(message));
  pb_encode(&sendStream, Potentiometer_Brightness_fields, data);
  *buffer = message;
  *length = sendStream.bytes_written;
}

