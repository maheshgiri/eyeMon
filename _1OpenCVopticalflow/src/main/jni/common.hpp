#ifndef COMMON_H
#define COMMON_H

#include <chrono>
#include <list>

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <blinkmeasure.hpp>
#include <blinkmeasuref.hpp>

void doLog(bool shouldPrint, const std::string fmt, ...);
void diffclock(char const *title, clock_t clock2);
void difftime(char const *title, std::chrono::time_point<std::chrono::steady_clock> t2, bool shouldExecute);
void doLogClock(const char* format, const char* title, double diffms);
void doLogClock1(const char* format, const char* title, double diffms);
void imshowWrapper(const char* name, cv::Mat mat, bool shouldShow);
void printStatus();

#define DEBUG_LEVEL 3
#define DEBUG_TAG "NDK_AndroidNDK1SampleActivity"

#define METHOD_OPTFLOW 0
#define METHOD_TEMPLATE_BASED 1
#define METHOD_BLACK_PIXELS 2
#define METHOD_FARNEBACK 3
#define METHOD_BLACKPIXELS 4

extern int PHONE;
extern std::chrono::high_resolution_clock::time_point startx;
extern int method;
extern int pauseFrames;
extern double previousFrameTime;

struct annotEyePosition {
    int l1x, l1y, l2x, l2y;
    int r1x, r1y, r2x, r2y;
};
struct activeSlice {
    double start, end;
};

struct stateMachineElement {
    BlinkMeasureF bm;
    double mlsdt, plsdt, mrsdt, prsdt;
};

struct stateMachineElementT {
    BlinkMeasure bm;
    double lsdt, rsdt;
};





extern std::map<int, annotEyePosition> annotEyePositionMap;
extern std::map<int, double> annotTimestampsMap;
extern bool shouldUseAnnotEyePosition;
extern bool shouldUseAnnotTimestamps;

extern bool debug_show_img_d1;
extern bool debug_show_img_main;
extern bool debug_show_img_gray;
extern bool debug_show_img_face;
extern bool debug_show_img_optfl_eyes;
extern bool debug_show_img_templ_eyes_cor;
extern bool debug_show_img_templ_eyes_tmpl;
extern bool debug_show_img_farne_eyes;
extern bool debug_t1_log;
extern bool debug_t2_log;
extern bool debug_t2_perf_method;
extern bool debug_t2_perf_whole;
extern bool debug_t2_perf;
extern bool debug_tmpl_log;
extern bool debug_tmpl_perf1;
extern bool debug_tmpl_perf2;
extern bool debug_tmpl_perfa;
extern bool debug_fb_log1;
extern bool debug_fb_log_flow;
extern bool debug_fb_log_tracking;
extern bool debug_fb_log_upperlowerdiff;
extern bool debug_fb_log_reinit;
extern bool debug_fb_log_repupil;
extern bool debug_fb_log_repupil1;
extern bool debug_fb_log_pupil_coverage;
extern bool debug_fb_perf1;
extern bool debug_fb_perf2;
extern bool debug_fb_perfa;
extern bool debug_bp_log_pix;
extern bool debug_notifications_log1;
extern bool debug_notifications_n1_log1;
extern bool debug_blinks_d1; // log last, avg, SD
extern bool debug_blinks_d2; // shortBmSize
extern bool debug_blinks_d3; // progress of blink detection state machine
extern bool debug_blinks_d4; // adding chuncks
extern bool debug_blinks_d5; // joined chuncks
extern bool debug_blink_beeps;
extern bool debug_n1_beeps;

// optical flow
extern int flg;

extern int farne;

extern int leftXOffset, leftYOffset, leftCols, leftRows;
extern int rightXOffset, rightYOffset, rightCols, rightRows;

extern int leftXp1, leftYp1, rightXp1, rightYp1;
extern int leftXlast, leftYlast, rightXlast, rightYlast;
extern int leftXavg, leftYavg, rightXavg, rightYavg;
extern int eye_region_width, eye_region_height;

extern cv::TermCriteria termcrit;
extern cv::Size subPixWinSize, winSize;
extern const int MAX_COUNT;
extern bool addRemovePtx;
extern cv::vector<cv::Point2f> points[2];
extern cv::Mat pleft, pright;
extern cv::Mat toSave;
extern int firstLoopProcs;
extern cv::CascadeClassifier face_cascade;

///
extern int maxFramesShortList;
extern std::map<unsigned int, double> toChunksLeft;
extern std::map<unsigned int, double> toChunksRight;


extern std::list<BlinkMeasure> blinkMeasure;
extern std::list<BlinkMeasure> blinkMeasureShort;
extern std::list<Blink> lBlinkChunks;
extern std::list<Blink> rBlinkChunks;
extern std::list<Blink> joinedBlinkChunksN1;

extern std::list<struct stateMachineElement> stateMachineQueue;
extern std::list<struct stateMachineElementT> stateMachineQueueT;

extern std::list<Blink> lBlinkTimeframeChunks;
extern std::list<Blink> rBlinkTimeframeChunks;
extern std::list<struct activeSlice> n1ActiveSlices;

/// farne
extern int maxFramesShortListf;
extern std::list<BlinkMeasureF> blinkMeasuref;
extern std::list<BlinkMeasureF> blinkMeasureShortf;
extern std::list<BlinkF> lBlinkChunksf;
extern std::list<BlinkF> rBlinkChunksf;
extern std::list<BlinkF> joinedBlinkChunksfN1;

extern std::list<BlinkF> lBlinkTimeframeChunksf;
extern std::list<BlinkF> rBlinkTimeframeChunksf;

#endif
