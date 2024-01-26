#include <vector>
#include <cstring>
#include <cstdint> 
#include "FloatPacketType.h"

/*std::vector<double> downstream;
std::vector<double> upstream;
bool oneShotMode = false;
bool oneShotDone = false;
int numberOfBytesPerValue = 4;
int idOfCommand = 0;
bool done = false;
bool started = false;
int packetSize = 64;
int numValues = packetSize / 4 - 1;*/



std::vector<double> returnValues;

std::vector<unsigned char> message;


std::vector<double> FloatPacketType::getUpstream() {
    return upstream;
}

void FloatPacketType::setUpstream(std::vector<double> upstream) {
    upstream = upstream;
}

std::vector<double> FloatPacketType::getDownstream() {
    return downstream;
}

void FloatPacketType::setDownstream(std::vector<double> downstream) {
    downstream = downstream;
}

FloatPacketType::FloatPacketType(int id, int size) {
    //super(id)
    packetSize = size;
    numberOfBytesPerValue = 4;
    numValues = packetSize / numberOfBytesPerValue - 4 / numberOfBytesPerValue;
    std::vector<double> retval(numValues, 0);
    returnValues = retval;
    std::vector<unsigned char> msg(packetSize, 0);
    message = msg;
    std::vector<double> downst(numValues, 0);
    setDownstream(downst);
    std::vector<double> upst(numValues, 0);
    setUpstream(upst);
    for (int i = 0; i < numValues; i++) {
        getDownstream()[i] = 0.0;
        getUpstream()[i] = 0.0;
    } 
}

std::vector<double> FloatPacketType::parse(std::vector<unsigned char> bytes) {
    for (int i = 0; i < numValues; i++) {
        int baseIndex = 4 * i + 4;
        int bits = toInt(bytes[0 + baseIndex]) | toInt(bytes[1 + baseIndex]) << 8 | toInt(bytes[2 + baseIndex]) << 16 | toInt(bytes[3 + baseIndex]) << 24;
        returnValues[i] = (float) (intBitsToFloat(bits));
    } 
    return returnValues;
}

int FloatPacketType::toInt(unsigned char byteValue) {
    int val = byteValue;
    if (val < 0)
    val += 256; 
    return val;
}

void FloatPacketType::writeId(int idOfCommand, std::vector<unsigned char> bytes) {
    //4 bytes = int
    bytes[3] = (unsigned char)(idOfCommand >> 24);
    bytes[2] = (unsigned char)(idOfCommand >> 16);
    bytes[1] = (unsigned char)(idOfCommand >> 8);
    bytes[0] = (unsigned char)idOfCommand; //corresponds with first byte being the reportid
}

std::vector<unsigned char> FloatPacketType::command(int idOfCommand, std::vector<double> values) {
    writeId(idOfCommand, message);
    for (int i = 0; i < numValues && i < values.size(); i++) {
        int baseIndex = 4 * i + 4;
        int bits = float_to_bits(((float)values[i])); //don't want to
        message[0 + baseIndex] = (unsigned char)(bits & 0xFF);
        message[1 + baseIndex] = (unsigned char)(bits >> 8 & 0xFF);
        message[2 + baseIndex] = (unsigned char)(bits >> 16 & 0xFF);
        message[3 + baseIndex] = (unsigned char)(bits >> 24 & 0xFF);
    } 
    return message;
}

/**
 * TODO: not sure if this is correct for getting the id from the bytebuffer thing
*/
int FloatPacketType::getId(std::vector<unsigned char> bytes) {
    //ByteBuffer 
    /*unsigned char* bAry = calloc(1024,1); //remember to free

    memcpy(bAry, &length, sizeof(int));    
    memcpy(bAry + sizeof(int), fAry, length*(sizeof(float)));*/

    //looks fancy but I'm guessing I can just do bytes[0]
    //return ByteBuffer.wrap(bytes).order(be).getInt(0); //ByteBuffer given array bytes, 
        //with byte order little endian
        //returns int from buffer's current position
    
    return bytes[0]; //I'm gonna seethe and cope if it turns out it's not in little endian or something
}



/**
 * intBitsToFloat conversion from java
*/
float FloatPacketType::intBitsToFloat(int32_t int_value)
{
    FloatPacketType::int_to_float_bits bits;
    bits.integer_bits = int_value;
    return bits.converted_float_bits;
}

/**
 * floatToIntBits
*/
unsigned FloatPacketType::float_to_bits(float x)
{
    unsigned y;
    memcpy(&y, &x, 4);
    return y;
}

bool FloatPacketType::sendOk(){
    if (!FloatPacketType::oneShotMode){
        return true; 
    }
    if (!FloatPacketType::oneShotDone) {
        FloatPacketType::oneShotDone = true;
        return true;
    }
    return false;
}
