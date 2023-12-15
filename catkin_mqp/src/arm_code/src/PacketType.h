#include <vector>

//might just make everything double
class PacketType{
    
    
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
    PacketType(int id){}

    std::vector<double> parse(std::vector<unsigned char> paramArrayOfbyte);

    static int getId(std::vector<unsigned char>);

    std::vector<double> getUpstream();

    void setUpstream(std::vector<double> upstream);
    
    std::vector<double> getDownstream();
    
    void setDownstream(std::vector<double> downstream);
};