#include "gestures.h"
#include <iostream>

namespace dstrss
{
	GESTURE_EVENT State::ChangeState(const GESTURE_PROPERTY property)
	{
		GESTURE_EVENT event = GESTURE_EVENT::STAY;
		switch (property)
		{
		case GESTURE_PROPERTY::MOUTH_OPEN:
			if (this->Page != State::PAGE::TEMPERATURE)
			{
				this->Page = State::PAGE::TEMPERATURE;
				event = GESTURE_EVENT::VIEW_TEMP_PAGE;
			}
			break;
		case GESTURE_PROPERTY::FACE_TILT_RIGHT:
			if (this->Page != State::PAGE::FAN)
			{
				this->Page = State::PAGE::FAN;
				event = GESTURE_EVENT::VIEW_FAN_PAGE;
			}
			break;
		case GESTURE_PROPERTY::FACE_TURN_DOWN:
			if (this->Page == State::PAGE::FAN)
			{
				if (this->FanVal == 255)
				{
					event = GESTURE_EVENT::SET_FAN_VALUE_100;
					this->FanVal = 100;
				}
				else if (this->FanVal == 100)
				{
					event = GESTURE_EVENT::SET_FAN_VALUE_0;
					this->FanVal = 0;
				}
			}
			break;
		case GESTURE_PROPERTY::FACE_TURN_UP:
			if (this->Page == State::PAGE::FAN)
			{
				if (this->FanVal == 100)
				{
					event = GESTURE_EVENT::SET_FAN_VALUE_255;
					this->FanVal = 255;
				}
				else if (this->FanVal == 0)
				{
					event = GESTURE_EVENT::SET_FAN_VALUE_100;
					this->FanVal = 100;
				}
			}
			break;
		}
		return event;
	}

	Gestures::Gestures(const std::string& url) :
		threshold_(20),
		curl_initialized_(false)
	{
		// initialize libcurl
		this->url_ = url;
		this->curl_.reset(curl_easy_init());
		if (this->curl_ == NULL) {
			std::cerr << "curl_easy_init() failed" << std::endl;
		}
		else
		{
			this->curl_initialized_ = true;
		}
	}

	Gestures::~Gestures(void)
	{
		this->curl_.reset();
	}

	bool Gestures::SendControlMessage(const std::string& option)
	{
		if (!this->curl_initialized_)
		{
			return false;
		}

		std::string message = this->url_ + option;

		int len = message.length();
		std::shared_ptr<char> postthis(new char[len + 1]);
		memcpy(postthis.get(), message.c_str(), len + 1);

		curl_easy_setopt(this->curl_.get(), CURLOPT_URL, postthis.get());
		auto ret = curl_easy_perform(this->curl_.get());

		if (ret != CURLE_OK) {
			std::cerr << "curl_easy_perform() failed." << std::endl;
			return false;
		}
		return true;
	}

	GESTURE_PROPERTY Gestures::EstimageGesture(const int pitch, const int yaw, const int roll) const
	{
		GESTURE_PROPERTY property = GESTURE_PROPERTY::NONE;
		if (this->IsFaceTurnRight(yaw))
		{
			property = GESTURE_PROPERTY::FACE_TURN_RIGHT;
		}
		else if (this->IsFaceTurnLeft(yaw))
		{
			property = GESTURE_PROPERTY::FACE_TURN_LEFT;
		}
		else if (this->IsFaceTurnDown(pitch))
		{
			property = GESTURE_PROPERTY::FACE_TURN_DOWN;
		}
		else if (this->IsFaceTurnUp(pitch))
		{
			property = GESTURE_PROPERTY::FACE_TURN_UP;
		}
		else if (this->IsFaceTiltRight(roll))
		{
			property = GESTURE_PROPERTY::FACE_TILT_RIGHT;
		}
		else if (this->IsFaceTiltLeft(roll))
		{
			property = GESTURE_PROPERTY::FACE_TILT_LEFT;
		}
		return property;
	}


	bool Gestures::IsFaceTurnRight(const int yaw) const
	{
		return yaw < -1 * this->threshold_ ? true : false;
	}

	bool Gestures::IsFaceTurnLeft(const int yaw) const
	{
		return yaw > this->threshold_ ? true : false;
	}

	bool Gestures::IsFaceTurnUp(const int pitch) const
	{
		return pitch > this->threshold_ ? true : false;
	}

	bool Gestures::IsFaceTurnDown(const int pitch) const
	{
		return pitch < -1 * this->threshold_ ? true : false;
	}

	bool Gestures::IsFaceTiltRight(const int roll) const
	{
		return roll < -1 * this->threshold_ ? true : false;
	}

	bool Gestures::IsFaceTiltLeft(const int roll) const
	{
		return roll > this->threshold_ ? true : false;
	}

	bool Gestures::GestureEventArise(const GESTURE_PROPERTY property)
	{
		return this->GestureEventArise(this->state_.ChangeState(property));
	}

	bool Gestures::GestureEventArise(const GESTURE_EVENT event)
	{
		std::string message = "";
		bool flag;
		switch (event)
		{
		case GESTURE_EVENT::VIEW_TEMP_PAGE:
			message = "page/temp";
			break;
		case GESTURE_EVENT::VIEW_FAN_PAGE:
			flag = true;
			if (this->state_.FanVal == 0)
			{
				this->state_.FanVal = 100;
				flag = this->GestureEventArise(GESTURE_EVENT::SET_FAN_VALUE_100);
			}
			if (flag)
			{
				message = "page/fan";
			}
			break;
		case GESTURE_EVENT::SET_FAN_VALUE_0:
			message = "motor/0";
			break;
		case GESTURE_EVENT::SET_FAN_VALUE_100:
			message = "motor/100";
			break;
		case GESTURE_EVENT::SET_FAN_VALUE_255:
			message = "motor/255";
			break;
		default:
			break;
		}
		if (!message.empty())
		{
			return this->SendControlMessage(message);
		}
		return true;
	}
}