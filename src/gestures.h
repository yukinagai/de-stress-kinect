#ifndef __GESTURES_H__
#define __GESTURES_H__

#include <string>
#include <curl/curl.h>
#include <memory>

namespace dstrss
{

	enum GESTURE_PROPERTY
	{
		FACE_TURN_RIGHT,
		FACE_TURN_LEFT,
		FACE_TURN_UP,
		FACE_TURN_DOWN,
		FACE_TILT_RIGHT,
		FACE_TILT_LEFT,
		MOUTH_OPEN,
		HAPPY,
		NONE
	};

	enum GESTURE_EVENT
	{
		VIEW_FAN_PAGE,
		VIEW_TEMP_PAGE,
		SET_FAN_VALUE_0,
		SET_FAN_VALUE_100,
		SET_FAN_VALUE_255,
		STAY
	};

	class State
	{
	public:
		State() : FanVal(0), TempVal(0), TimeCount(0), Page(INIT) {};
		~State(){};

		GESTURE_EVENT ChangeState(const GESTURE_PROPERTY property);

		enum PAGE
		{
			FAN,
			TEMPERATURE,
			INIT
		};

		int FanVal;
		int TempVal;
		unsigned TimeCount;
		PAGE Page;
	};

	class Gestures
	{
	private:
		std::string url_;
		int threshold_;
		std::shared_ptr<CURL> curl_;
		bool curl_initialized_;
		State state_;

	public:
		Gestures(const std::string& url);

		~Gestures(void);

		bool SendControlMessage(const std::string& option);

		GESTURE_PROPERTY EstimageGesture(const int pitch, const int yaw, const int roll) const;

		bool IsFaceTurnRight(const int yaw) const;
		bool IsFaceTurnLeft(const int yaw) const;
		bool IsFaceTurnUp(const int pitch) const;
		bool IsFaceTurnDown(const int pitch) const;
		bool IsFaceTiltRight(const int roll) const;
		bool IsFaceTiltLeft(const int roll) const;

		bool CurlIsInitialized(void) const
		{
			return this->curl_initialized_;
		}

		bool GestureEventArise(const GESTURE_PROPERTY property);
		bool GestureEventArise(const GESTURE_EVENT event);
	};

}
#endif // __GESTURES_H__