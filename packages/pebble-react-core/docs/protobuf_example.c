const char *sourceString = "[{\"operation\":\"1\",\"nodeType\":\"1\",\"nodeId\":\"12\",\"props\":{\"height\":\"30\",\"left\":\"0\",\"top\":\"-1000\",\"width\":\"25\",\"children\":\"R\"}},{\"operation\":\"1\",\"nodeType\":\"1\",\"nodeId\":\"14\",\"props\":{\"height\":\"30\",\"left\":\"30\",\"top\":\"-1000\",\"width\":\"25\",\"children\":\"e\"}},{\"operation\":\"1\",\"nodeType\":\"1\",\"nodeId\":\"16\",\"props\":{\"height\":\"30\",\"left\":\"60\",\"top\":\"-1000\",\"width\":\"25\",\"children\":\"a\"}},{\"operation\":\"1\",\"nodeType\":\"1\",\"nodeId\":\"18\",\"props\":{\"height\":\"30\",\"left\":\"90\",\"top\":\"-1000\",\"width\":\"25\",\"children\":\"c\"}},{\"operation\":\"1\",\"nodeType\":\"1\",\"nodeId\":\"110\",\"props\":{\"height\":\"30\",\"left\":\"120\",\"top\":\"-1000\",\"width\":\"25\",\"children\":\"t\"}},{\"operation\":\"1\",\"nodeType\":\"1\",\"nodeId\":\"112\",\"props\":{\"height\":\"30\",\"left\":\"-1000\",\"top\":\"80\",\"width\":\"25\",\"children\":\"R\"}},{\"operation\":\"1\",\"nodeType\":\"1\",\"nodeId\":\"114\",\"props\":{\"height\":\"30\",\"left\":\"-1000\",\"top\":\"80\",\"width\":\"25\",\"children\":\"o\"}},{\"operation\":\"1\",\"nodeType\":\"1\",\"nodeId\":\"116\",\"props\":{\"height\":\"30\",\"left\":\"-1000\",\"top\":\"80\",\"width\":\"25\",\"children\":\"c\"}},{\"operation\":\"1\",\"nodeType\":\"1\",\"nodeId\":\"118\",\"props\":{\"height\":\"30\",\"left\":\"-1000\",\"top\":\"80\",\"width\":\"25\",\"children\":\"k\"}},{\"operation\":\"1\",\"nodeType\":\"1\",\"nodeId\":\"120\",\"props\":{\"height\":\"30\",\"left\":\"-1000\",\"top\":\"80\",\"width\":\"25\",\"children\":\"s\"}},{\"operation\":\"1\",\"nodeType\":\"5\",\"nodeId\":\"53\",\"props\":{\"animationProps\":[{\"end\":\"50\",\"name\":\"top\",\"start\":\"-30\",\"type\":\"number\"}],\"duration\":\"750\",\"children\":[\"12\"],\"isSequence\":\"0\",\"animationsLength\":\"1\",\"childrenLength\":\"1\"}},{\"operation\":\"1\",\"nodeType\":\"5\",\"nodeId\":\"55\",\"props\":{\"animationProps\":[{\"end\":\"50\",\"name\":\"top\",\"start\":\"-60\",\"type\":\"number\"}],\"duration\":\"750\",\"children\":[\"14\"],\"isSequence\":\"0\",\"animationsLength\":\"1\",\"childrenLength\":\"1\"}},{\"operation\":\"1\",\"nodeType\":\"5\",\"nodeId\":\"57\",\"props\":{\"animationProps\":[{\"end\":\"50\",\"name\":\"top\",\"start\":\"-90\",\"type\":\"number\"}],\"duration\":\"750\",\"children\":[\"16\"],\"isSequence\":\"0\",\"animationsLength\":\"1\",\"childrenLength\":\"1\"}},{\"operation\":\"1\",\"nodeType\":\"5\",\"nodeId\":\"59\",\"props\":{\"animationProps\":[{\"end\":\"50\",\"name\":\"top\",\"start\":\"-120\",\"type\":\"number\"}],\"duration\":\"750\",\"children\":[\"18\"],\"isSequence\":\"0\",\"animationsLength\":\"1\",\"childrenLength\":\"1\"}},{\"operation\":\"1\",\"nodeType\":\"5\",\"nodeId\":\"511\",\"props\":{\"animationProps\":[{\"end\":\"50\",\"name\":\"top\",\"start\":\"-150\",\"type\":\"number\"}],\"duration\":\"750\",\"children\":[\"110\"],\"isSequence\":\"0\",\"animationsLength\":\"1\",\"childrenLength\":\"1\"}},{\"operation\":\"1\",\"nodeType\":\"5\",\"nodeId\":\"513\",\"props\":{\"animationProps\":[{\"end\":\"0\",\"name\":\"left\",\"start\":\"1000\",\"type\":\"number\"}],\"duration\":\"1500\",\"children\":[\"112\"],\"isSequence\":\"0\",\"animationsLength\":\"1\",\"childrenLength\":\"1\"}},{\"operation\":\"1\",\"nodeType\":\"5\",\"nodeId\":\"515\",\"props\":{\"animationProps\":[{\"end\":\"30\",\"name\":\"left\",\"start\":\"1000\",\"type\":\"number\"}],\"duration\":\"1500\",\"children\":[\"114\"],\"isSequence\":\"0\",\"animationsLength\":\"1\",\"childrenLength\":\"1\"}},{\"operation\":\"1\",\"nodeType\":\"5\",\"nodeId\":\"517\",\"props\":{\"animationProps\":[{\"end\":\"60\",\"name\":\"left\",\"start\":\"1000\",\"type\":\"number\"}],\"duration\":\"1500\",\"children\":[\"116\"],\"isSequence\":\"0\",\"animationsLength\":\"1\",\"childrenLength\":\"1\"}},{\"operation\":\"1\",\"nodeType\":\"5\",\"nodeId\":\"519\",\"props\":{\"animationProps\":[{\"end\":\"90\",\"name\":\"left\",\"start\":\"1000\",\"type\":\"number\"}],\"duration\":\"1500\",\"children\":[\"118\"],\"isSequence\":\"0\",\"animationsLength\":\"1\",\"childrenLength\":\"1\"}},{\"operation\":\"1\",\"nodeType\":\"5\",\"nodeId\":\"521\",\"props\":{\"animationProps\":[{\"end\":\"120\",\"name\":\"left\",\"start\":\"1000\",\"type\":\"number\"}],\"duration\":\"1500\",\"children\":[\"120\"],\"isSequence\":\"0\",\"animationsLength\":\"1\",\"childrenLength\":\"1\"}},{\"operation\":\"1\",\"nodeType\":\"3\",\"nodeId\":\"322\",\"props\":{\"name\":\"some\",\"test\":\"test\"}}]";

uint8_t buffer[10];
size_t messageLength;
bool status;

Encode
{
  Example messageToEncode = Example_init_zero;

  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

  messageToEncode.value = 13;

  int status = pb_encode(&stream, Example_fields, &messageToEncode);
  messageLength = stream.bytes_written;

  if (!status)
  {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Encoding failed: %s\n", PB_GET_ERROR(&stream));
    return 1;
  }
}

// Decode
{
  Example messageToDecode = Example_init_zero;

  pb_istream_t stream = pb_istream_from_buffer(buffer, messageLength);

  status = pb_decode(&stream, Example_fields, &messageToDecode);

  if (!status)
  {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Decoding failed: %s\n", PB_GET_ERROR(&stream));
    return 1;
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Your lucky number was %d!\n", (int)messageToDecode.value);
}