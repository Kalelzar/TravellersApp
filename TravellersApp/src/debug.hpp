#ifdef TRVL_DEBUG
#define LOG(MSG) std::clog<<__FILE__<<":"<<__func__<<"("<<__LINE__<<") "<<MSG<<std::endl
#else
#define LOG(MSG)
#endif //TRVL_DEBUG
