#include <jni.h>

#include <opencv2/core/core.hpp>

#include <templatebased.cpp>

using namespace cv;
using namespace std;

extern "C" {

long int grabberFrameNum = 0;
TemplateBased templBased;

JNIEXPORT jlong JNICALL Java_org_blatnik_eyemon_TemplateBasedJNI_templateBasedCreateObject
                                                                (JNIEnv * jenv, jclass, jstring jCascadeFileName) {
    const char *cascadeFileName = (jenv)->GetStringUTFChars(jCascadeFileName, 0);
    templBased.setup(cascadeFileName);
#ifdef IS_PHONE
    templBased.setJni(jenv);
#endif
    (jenv)->ReleaseStringUTFChars(jCascadeFileName, cascadeFileName);
    jlong result = 0;
    return result;
}

JNIEXPORT void JNICALL Java_org_blatnik_eyemon_TemplateBasedJNI_templateBasedDestroyObject(JNIEnv * jenv, jclass, jlong thiz) {
}

JNIEXPORT void JNICALL Java_org_blatnik_eyemon_TemplateBasedJNI_templateBasedDetect
                                                                (JNIEnv * jenv, jclass, jlong imageRGB, jlong imageGray) {
    Mat rgb = *((Mat*)imageRGB);
    Mat gray = *((Mat*)imageGray);
    long int tstp = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count();
    templBased.run(gray, rgb, tstp, grabberFrameNum);
    grabberFrameNum++;
}


} // end of extern C
