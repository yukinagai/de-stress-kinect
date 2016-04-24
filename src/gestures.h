#ifndef __GESTURES_H__
#define __GESTURES_H__

#include <string>
#include <curl/curl.h>
#include <memory>

namespace dstrss
{

	enum GESTURE_PROPERTY
	{
		FACE_TURN_RIGHT = 1,
		FACE_TURN_LEFT = 2,
		FACE_TURN_UP = -3,
		FACE_TURN_DOWN = -4,
		FACE_TILT_RIGHT = -5,
		FACE_TILT_LEFT = 6,
		MOUTH_OPEN = 7,
		HAPPY = -8,
		RIGHT_EYE_CLOSED = -9,
		LEFT_EYE_CLOSED = -10,
		NONE = -11
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

	enum PAGE
	{
		FAN,
		TEMPERATURE,
		INIT
	};

	std::string GesturePropertyToString(const GESTURE_PROPERTY property);

	std::string GestureEventToString(const GESTURE_EVENT event);

	std::string PageToString(const PAGE event);

	class State
	{
	private:
		unsigned time_count_;
		GESTURE_PROPERTY prev_property_;
		const unsigned transition_threshold_;
		bool update_time_count(const GESTURE_PROPERTY property);

	public:
		State() : FanVal(0), TempVal(0), Page(INIT), time_count_(0),
			//prev_property_(GESTURE_PROPERTY::NONE), transition_threshold_(5) {};
			prev_property_(GESTURE_PROPERTY::NONE), transition_threshold_(3) {};
		~State(){};

		GESTURE_EVENT ChangeState(const GESTURE_PROPERTY property);
		unsigned get_time_count(){ return time_count_; };

		int FanVal;
		int TempVal;

		PAGE Page;
		

	};

	class Gestures
	{
	private:
		std::string url_;
		const int threshold_;
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