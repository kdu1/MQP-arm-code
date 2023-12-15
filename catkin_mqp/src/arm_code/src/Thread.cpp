#include <pthread.h> //not supported on windows

class Thread
{
  public:
    Thread();
    virtual ~Thread();

    int start();
    int join();
    int detach();
    pthread_t self();

    virtual void* run() = 0;

    //Constructor
    Thread::Thread() : m_tid(0), m_running(0), m_detached(0) {}

    //Destructor
    Thread::~Thread()
    {
        if (m_running == 1 && m_detached == 0) {
            pthread_detach(m_tid);
        }
        if (m_running == 1) {
            pthread_cancel(m_tid);
        }
    }

    //run
    static void* runThread(AbstractSimpleComsDevice* arg)
    {
        return ((Thread*)arg)->run();
    }

    //run thread
    static void* runThread(void* arg);

    //start
    int Thread::start()
    {
        int result = pthread_create(&m_tid, NULL, runThread, this);
        if (result == 0) {
            m_running = 1;
        }
        return result;
    }

    int Thread::join()
    {
        int result = -1;
        if (m_running == 1) {
            result = pthread_join(m_tid, NULL);
            if (result == 0) {
                m_detached = 1;
            }
        }
        return result;
    }

    int Thread::detach()
    {
        int result = -1;
        if (m_running == 1 && m_detached == 0) {
            result = pthread_detach(m_tid);
            if (result == 0) {
                m_detached = 1;
            }
        }
        return result;
    }

    //get thread id
    pthread_t Thread::self() {
        return m_tid;
    }

  private:
    pthread_t  m_tid;
    int        m_running;
    int        m_detached;



};