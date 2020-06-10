#ifdef TRVL_DEBUG
#define Q(x) #x
#define QUOTE(x) Q(x)
/// LOGS the provided message to stderr but only if compiled for debug
#define LOG(TYPE, MSG)                                                         \
  std::clog << "[" << QUOTE(TYPE) << "] " << __FILE__ << ":" << __func__       \
            << "(" << __LINE__ << ") " << MSG << std::endl
#else
/// LOGS the provided message to stderr but only if compiled for debug
#define LOG(TYPE, MSG)
#endif // TRVL_DEBUG
