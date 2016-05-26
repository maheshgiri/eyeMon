
#include <list>
#include <thread>


//#include <optflow.cpp>
#include <templatebased.cpp>

#include <main_settings.hpp>

void getGray(Mat frame, Mat *gray) {
    // int resizeFactor = 1;
    //resize(frame, *output, Size(frame.size().width/resizeFactor, frame.size().height/resizeFactor));
    //cvtColor(frame, *gray, CV_BGR2GRAY);
    std::vector<cv::Mat> rgbChannels(3);
    cv::split(frame, rgbChannels);
    *gray = rgbChannels[2];
}

class FrameCarrier {
    public: Mat frame;
    public: double timestamp;

    public: FrameCarrier(Mat frame, double timestamp) {
        this->frame     = frame;
        this->timestamp = timestamp;

    }
};

std::list<FrameCarrier> frameList;
bool canAdd = true;
bool finished = false;
bool grabbing = true;

void captureFrames() {
    if (!stream1.isOpened()) {
        CV_Assert("T1 cam open failed");
    }

    if (isVideoCapture == true) {
        // resolutions 320, 240; 800, 448; 640, 480
        //stream1.set(CV_CAP_PROP_FRAME_WIDTH, 320); stream1.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
        stream1.set(CV_CAP_PROP_FRAME_WIDTH, 640); stream1.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
        //stream1.set(CV_CAP_PROP_FRAME_WIDTH, 1280); stream1.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
    }

    if (isVideoCapture == false) {
        doLog(debug_t1_log, "debug_t1_log: T1 video capture %f %f %f\n", stream1.get(CV_CAP_PROP_FRAME_WIDTH), stream1.get(CV_CAP_PROP_FRAME_HEIGHT), stream1.get(CV_CAP_PROP_FPS));
        doLog(debug_t1_log, "debug_t1_log: CAP_PROP_FPS %f\n", stream1.get(CV_CAP_PROP_FPS));
        doLog(debug_t1_log, "debug_t1_log: CAP_PROP_FRAME_COUNT %f\n", stream1.get(CV_CAP_PROP_FRAME_COUNT));
    }

    Mat frame;
    //std::chrono::time_point<std::chrono::steady_clock> t1 = std::chrono::steady_clock::now();
    double prevFrameMs = 0;
    while (grabbing) {
        if(!(stream1.read(frame))) {
            doLog(debug_t1_log, "debug_t1_log: No captured frame, exiting!\n");
            finished = true;
            return;
        }

        long int listSize = frameList.size();
        doLog(debug_t1_log, "size %ld\n", frameList.size());

        if (listSize >= maxSize && disable_max_size != true) {
            doLog(debug_t1_log, "T1 reached max size %d\n", maxSize);
            canAdd = false;
        } else {
            if (canAdd == true) {
                double frameTimeMs;
                if (isVideoCapture == true) {
                    long int ft = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count();
                    frameTimeMs = (double) ft;
                } else {
                    frameTimeMs = (double) stream1.get(CV_CAP_PROP_POS_MSEC);
                }
                FrameCarrier fc(frame.clone(), frameTimeMs);
                frameList.push_back(fc);
                doLog(debug_t1_log, "debug_t1_log: frameTime %lf diff %lf\n", frameTimeMs, frameTimeMs-prevFrameMs);
                prevFrameMs = frameTimeMs;
            }
        }
    }
}

//OptFlow optf;
TemplateBased templ;

void doProcessing() {
    //cv::namedWindow(face_window_name,CV_WINDOW_NORMAL); cv::moveWindow(face_window_name, 10, 100);
    if (debug_show_img_main == true) {
        cv::namedWindow("main",CV_WINDOW_NORMAL); cv::moveWindow("main", 400, 100); resizeWindow("main",1280, 960);
    }
    if (debug_show_img_face == true) {
        cv::namedWindow("face",CV_WINDOW_NORMAL); cv::moveWindow("face", 400, 100);
    }
    if (debug_show_img_templ_eyes_tmpl == true) {
        cv::namedWindow("left",CV_WINDOW_NORMAL); cv::moveWindow("left", 1300, 500);
        cv::namedWindow("right",CV_WINDOW_NORMAL); cv::moveWindow("right", 1600, 500);
    }
    if (debug_show_img_templ_eyes_cor == true) {
        cv::namedWindow("leftR",CV_WINDOW_NORMAL); cv::moveWindow("leftR", 1300, 800);
        cv::namedWindow("rightR",CV_WINDOW_NORMAL); cv::moveWindow("rightR", 1600, 800);
    }
    /*
    cv::namedWindow("leftR1",CV_WINDOW_NORMAL); cv::moveWindow("leftR1", 10, 800);
    cv::namedWindow("rightR1",CV_WINDOW_NORMAL); cv::moveWindow("rightR1", 200, 800);
    */
    // cv::namedWindow("Right Eye",CV_WINDOW_NORMAL); cv::moveWindow("Right Eye", 10, 600);
    // cv::namedWindow("Left Eye",CV_WINDOW_NORMAL); cv::moveWindow("Left Eye", 10, 800);
    // createCornerKernels(), at the end // releaseCornerKernels(); // ellipse(skinCrCbHist, cv::Point(113, 155.6), cv::Size(23.4, 15.2), 43.0, 0.0, 360.0, cv::Scalar(255, 255, 255), -1);

    std::chrono::time_point<std::chrono::steady_clock> t1 = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> t2;
    Mat frame, gray, cflow;

    while (true) {
        long unsigned int listSize = frameList.size();
        if (listSize == 0) {
            if (finished == true) {
                doLog(true, "exiting\n");
                break;
            }
            if (canAdd == false) {
                canAdd = true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        FrameCarrier fc = frameList.front();
        frameList.pop_front();
        Mat frame = fc.frame;
        // cv::flip(frame, frame, 1);
        double timestamp = fc.timestamp;
        if (debug_t2_log == true) {
            doLog(true, "T2 frame time: %lf\n", timestamp);
        }

        t2 = std::chrono::steady_clock::now();
        switch (method) {
            case METHOD_OPTFLOW:
            case METHOD_TEMPLATE_BASED:
            getGray(frame, &gray);
            break;
            case METHOD_BLACK_PIXELS:
            break;
        }
        difftime("T2 getGray", t2, debug_t2_log);

        t2 = std::chrono::steady_clock::now();
        switch (method) {
            case METHOD_OPTFLOW:
            //optf.run(gray, frame);
            break;
            case METHOD_TEMPLATE_BASED:
            templ.run(gray, frame, timestamp);
            break;
            case METHOD_BLACK_PIXELS:
            break;
        }
        difftime("T2 run", t2, debug_t2_perf_method);


        if (debug_show_img_main == true) {
            // flow control
            int c = cv::waitKey(10);
            if((char)c == 'q') {
                doLog(true, "exiting\n");
                grabbing = false;
                break;
            } else if((char)c == 'p') {
                pause = 1;
            } else if((char)c == 'f') {
                flg = 1;
                // imwrite("/tmp/frame.png",cflow);
            } else if (pause == 1) {
                while (true) {
                    int c = cv::waitKey(10);
                    if((char)c == 'p') {
                        pause = 0;
                        break;
                    } else if((char)c == 'n') {
                        break;
                    } else if((char)c == 's') {
                        // status
                        printStatus();
                        break;
                    }
                }
            }
        }
        difftime("T2 whole loop:", t1, debug_t2_perf_whole);
        t1 = std::chrono::steady_clock::now();
    }
}
///
char* getCmdOption(char** begin, char** end, const std::string& option) {
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }

    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}
///
void processOptions(int argc, char* argv[]) {
    /*
    if(cmdOptionExists(argv, argv+argc, "-h")) {
        // Do stuff
    }
    */
    char* filename = getCmdOption(argv, argv + argc, "--debug_show_img_main");
    if (filename) {
        // Do interesting things
    }
}

int main() { // int argc, char * argv[]
    //processOptions(argc, argv);
    //return 0;
    PHONE = 0;

    char faceDetector[200] = "/home/developer/other/android_deps/OpenCV-2.4.10-android-sdk/samples/optical-flow/res/raw/lbpcascade_frontalface.xml";
    switch (method) {
        case METHOD_OPTFLOW:
        //optf.setup(faceDetector);
        break;
        case METHOD_TEMPLATE_BASED:
        templ.setup(faceDetector);
        break;
        case METHOD_BLACK_PIXELS:
        break;
    }

    thread t1(captureFrames);
    thread t2(doProcessing);

    t1.join();
    t2.join();

    return 0;
}
