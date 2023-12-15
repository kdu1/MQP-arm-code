#include <vector>
#include <cstring>
#include <cstdint> 

class FloatPacketType{
    private:
        std::vector<double> downstream;
        std::vector<double> upstream;
        bool oneShotMode = false;
        
        
    protected:
        //ByteOrder be = ByteOrder.LITTLE_ENDIAN;//don't think I need it?
        int numberOfBytesPerValue = 4;
        bool oneShotDone = false;
  
    public:
        int idOfCommand = 0;
        bool done = false;
        bool started = false;
        int packetSize = 64;
        int numValues = packetSize / 4 - 1;

    std::vector<double> returnValues;
  
    std::vector<unsigned char> message;

    std::vector<double> getUpstream() {
        return this->upstream;
    }

    void setUpstream(std::vector<double> upstream) {
        this->upstream = upstream;
    }
    
    std::vector<double> getDownstream() {
        return this->downstream;
    }
    
    void setDownstream(std::vector<double> downstream) {
        this->downstream = downstream;
    }
  
    FloatPacketType(int id, int size) {
        //super(id)
        packetSize = size;
        this->numberOfBytesPerValue = 4;
        this->numValues = packetSize / this->numberOfBytesPerValue - 4 / this->numberOfBytesPerValue;
        std::vector<double> retval(this->numValues, 0);
        this->returnValues = retval;
        std::vector<unsigned char> msg(packetSize, 0);
        this->message = msg;
        std::vector<double> downst(this->numValues, 0);
        setDownstream(downst);
        std::vector<double> upst(this->numValues, 0);
        setUpstream(upst);
        for (int i = 0; i < this->numValues; i++) {
            getDownstream()[i] = 0.0;
            getUpstream()[i] = 0.0;
        } 
    }
  
    std::vector<double> parse(std::vector<unsigned char> bytes) {
        for (int i = 0; i < this->numValues; i++) {
            int baseIndex = 4 * i + 4;
            int bits = toInt(bytes[0 + baseIndex]) | toInt(bytes[1 + baseIndex]) << 8 | toInt(bytes[2 + baseIndex]) << 16 | toInt(bytes[3 + baseIndex]) << 24;
            this->returnValues[i] = (float) (intBitsToFloat(bits));
        } 
        return this->returnValues;
    }
  
    int toInt(unsigned char byteValue) {
        int val = byteValue;
        if (val < 0)
        val += 256; 
        return val;
    }

    void writeId(int idOfCommand, std::vector<unsigned char> bytes) {
        //4 bytes = int
        bytes[3] = (unsigned char)(idOfCommand >> 24);
        bytes[2] = (unsigned char)(idOfCommand >> 16);
        bytes[1] = (unsigned char)(idOfCommand >> 8);
        bytes[0] = (unsigned char)idOfCommand; //corresponds with first byte being the reportid
    }
  
    std::vector<unsigned char> command(int idOfCommand, std::vector<double> values) {
        writeId(idOfCommand, this->message);
        for (int i = 0; i < this->numValues && i < values.size(); i++) {
            int baseIndex = 4 * i + 4;
            int bits = float_to_bits(((float)values[i])); //don't want to
            this->message[0 + baseIndex] = (unsigned char)(bits & 0xFF);
            this->message[1 + baseIndex] = (unsigned char)(bits >> 8 & 0xFF);
            this->message[2 + baseIndex] = (unsigned char)(bits >> 16 & 0xFF);
            this->message[3 + baseIndex] = (unsigned char)(bits >> 24 & 0xFF);
        } 
        return this->message;
    }

    /**
     * TODO: not sure if this is correct for getting the id from the bytebuffer thing
    */
    static int getId(std::vector<unsigned char> bytes) {
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

    union int_to_float_bits {
        int32_t integer_bits;
        float converted_float_bits;
    };

    /**
     * intBitsToFloat conversion from java
    */
    float intBitsToFloat(int32_t int_value)
    {
        union int_to_float_bits bits;
        bits.integer_bits = int_value;
        return bits.converted_float_bits;
    }

    /**
     * floatToIntBits
    */
    unsigned float_to_bits(float x)
    {
        unsigned y;
        memcpy(&y, &x, 4);
        return y;
    }

    bool sendOk(){
        if (!this->oneShotMode){
            return true; 
        }
        if (!this->oneShotDone) {
            this->oneShotDone = true;
            return true;
        }
        return false;
    }
};
