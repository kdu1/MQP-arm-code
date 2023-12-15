#include <vector>
#include <cstdint> 

//every PacketType will just be FloatPacketType
class FloatPacketType{
    private:
        std::vector<double> downstream;
        std::vector<double> upstream;
        bool oneShotMode = false;
        bool oneShotDone = false;
        
    protected:
        //ByteOrder be = ByteOrder.LITTLE_ENDIAN;//don't think I need it?
        int numberOfBytesPerValue = 4;
  
    public:
        int idOfCommand = 0;
        bool done = false;
        bool started = false;
        int packetSize = 64;
        int numValues = packetSize / 4 - 1;

    FloatPacketType(int id, int size);
    
    std::vector<double> returnValues;
  
    std::vector<unsigned char> message;

    std::vector<double> getUpstream();

    void setUpstream(std::vector<double> upstream);

    std::vector<double> getDownstream();

    void setDownstream(std::vector<double> downstream);

    std::vector<double> parse(std::vector<unsigned char> bytes);

    int toInt(unsigned char byteValue);

    void writeId(int idOfCommand, std::vector<unsigned char> bytes);

    std::vector<unsigned char> command(int idOfCommand, std::vector<double> values);

    static int getId(std::vector<unsigned char>);

    bool sendOk(); 

};