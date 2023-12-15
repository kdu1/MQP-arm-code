#include <vector>
#include "PacketType.h"
//might just make everything double
class PacketType{
    
    private:
        std::vector<double> downstream;
        std::vector<double> upstream;
        bool oneShotMode = false;
        bool oneShotDone = false;
        
    protected:
        //ByteOrder be = ByteOrder.LITTLE_ENDIAN;
        int numberOfBytesPerValue = 4;

    public:
        int idOfCommand = 0;
        bool done = false;
        bool started = false;
        int packetSize = 64;
        int numValues = packetSize / 4 - 1;
  

    PacketType(int id){
        this->idOfCommand = id;
    }

    std::vector<double> parse(std::vector<unsigned char> paramArrayOfbyte){
        //there's no implementation
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
    
    void writeId(int idOfCommand, std::vector<unsigned char> bytes) {
        //4 bytes = int
        bytes[3] = (unsigned char)(idOfCommand >> 24);
        bytes[2] = (unsigned char)(idOfCommand >> 16);
        bytes[1] = (unsigned char)(idOfCommand >> 8);
        bytes[0] = (unsigned char)idOfCommand; //corresponds with first byte being the reportid
    }

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


};