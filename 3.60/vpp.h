/**
	@file
	@brief C++ wrapper for Haption's VirtuoseAPI

	Generated by parsing version 3.60 of
	virtuoseAPI.h

	@date
	2010-2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2010-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935
#define INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935

// Internal Includes
// none

// Library/third-party includes
#include <virtuoseAPI.h>

// Standard includes
#include <exception>
#include <stdexcept>
#include <string>

#ifdef VPP_VERBOSE
#	include <iostream>
#	define VPP_VERBOSE_MESSAGE(_MSG) std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": " << _MSG << std::endl
#else
#	define VPP_VERBOSE_MESSAGE(_MSG)
#endif

#ifndef VPP_DISABLE_ERROR_CHECK
#	include <sstream>
#endif

#define VIRTUOSEAPI_VERSION 3060

#define VIRTUOSEAPI_VERSION_CHECK(MAJOR, MINOR) (VIRTUOSEAPI_VERSION >= (MAJOR * 1000 + MINOR))

class Virtuose {
	public:
		struct VirtuoseAPIError : public std::runtime_error {
			VirtuoseAPIError(std::string const& what) : std::runtime_error(what) {}
		};


	private:
#ifndef VPP_DISABLE_ERROR_CHECK
		void _checkReturnCode(int returnValue, const char * call, const char * file,
		                      int const line, const char * func = "") {
			if (returnValue != 0) {
				std::ostringstream s;
				s << "VirtuoseAPI Error (in call '" << call << "' in " << func << "@" << file << ":" << line << "): " << getErrorMessage();
				throw VirtuoseAPIError(s.str());
			}
		}
#	define VPP_CHECKED_CALL(_CALL) _checkReturnCode(_CALL, #_CALL, __FILE__, __LINE__, __FUNCTION__)
#else
#	define VPP_CHECKED_CALL(_CALL)
#endif

	public:

		/** @brief constructor

			Opens a new VirtContext upon construction.

			@param name Name of Virtuose device to connect to.

			@throws VirtuoseAPIError if opening the device failed.
		*/
		Virtuose(const std::string & name)
			: _name(name)
			, _vc(virtOpen(_name.c_str()))
			, _weOpened(true) {
			VPP_VERBOSE_MESSAGE("Constructing a new Virtuose object, device named " << _name << ", VirtContext=" << _vc);
			if (_vc == NULL) {
				_weOpened = false;
				throw VirtuoseAPIError("Failed opening Virtuose " + _name + getErrorMessage());
			}
		}

		/// @brief Constructor from an existing Virtuose
		Virtuose(VirtContext vc)
			: _name("n/a")
			, _vc(vc)
			, _weOpened(false) {
			VPP_VERBOSE_MESSAGE("Constructing a Virtuose object from existing VirtContext " << _vc);
		}

		/// @brief Copy constructor
		Virtuose(Virtuose const& other)
			: _name(other._name)
			, _vc(other._vc)
			, _weOpened(false) {
			VPP_VERBOSE_MESSAGE("Copy-constructing a Virtuose object from existing Virtuose object, named " << _name << ", VirtContext=" << _vc);
		}

		/// @brief Assignment operator
		Virtuose & operator=(Virtuose const& other) {
			if (&other == this) {
				// Self assignment - no-op
				VPP_VERBOSE_MESSAGE("Self-assignment is a no-op");
			} else {
				// Actual assignment
				if (_weOpened) {
					// Close our existing one first.
					VPP_VERBOSE_MESSAGE("In assignment operator, closing existing Virtuose device named " << _name << ", VirtContext=" << _vc);
					VPP_CHECKED_CALL(virtClose(_vc));
				}
				_name = other._name;
				_vc = other._vc;
				_weOpened = false;
				VPP_VERBOSE_MESSAGE("Assignment operator has set ourselves to a device named " << _name << ", VirtContext=" << _vc);
			}
			return *this;
		}

		/** @brief destructor that closes the connection to the Virtuose
			device.
		*/
		~Virtuose() {
			if (_weOpened) {
				VPP_VERBOSE_MESSAGE("In destructor for device named " << _name << ", VirtContext=" << _vc << ", closing because _weOpened flag is set");
				try {
					VPP_CHECKED_CALL(virtClose(_vc));
				} catch (VirtuoseAPIError & e) {
					VPP_VERBOSE_MESSAGE("Exception in destructor, ignoring: " << e.what());
				}
			} else {
				VPP_VERBOSE_MESSAGE("In destructor for device named " << _name << ", VirtContext=" << _vc << ", NOT closing because _weOpened flag is not set");
			}
		}

		/** @brief Conversion operator to type VirtContext.
		*/
		operator VirtContext() {
			return _vc;
		}

		int attachVO(float mass, float * mxmymz);
		int attachQSVO(float * Ks, float * Bs);
		int attachVOAvatar(float mass, float * mxmymz);
		int detachVO();
		int detachVOAvatar();
		int displayHardwareStatus(FILE * fh);
		int getBaseFrame(float * base);
		int getButton(int button_number, int * state);
		int getCommandType(VirtCommandType * type);
		int getDeadMan(int * dead_man);
		int getEmergencyStop(int * emergency_stop);
		int getError(int * error);
		int getErrorCode();
		int getForce(float * force);
		int getForceFactor(float * force_factor);
		int getIndexingMode(VirtIndexingType * indexing_mode);
		int getLimitTorque(float * torque);
		int getObservationFrame(float * obs);
		int getPosition(float * pos);
		int getPowerOn(int * power);
		int getSpeed(float * speed);
		int getSpeedFactor(float * speed_factor);
		int getTimeLastUpdate(int unsigned * time);
		int getTimeoutValue(float * time_value);
		int setBaseFrame(float * base);
		int setCommandType(VirtCommandType type);
		int setDebugFlags(short unsigned flag);
		int setForce(float * force);
		int setForceFactor(float force_factor);
		int setIndexingMode(VirtIndexingType indexing_mode);
		int setLimitTorque(float torque);
		int setObservationFrame(float * obs);
		int setObservationFrameSpeed(float * speed);
		int setOutputFile(char * name);
		int setPeriodicFunction(void (* fn ) (VirtContext, void *), float * period, void * arg);
		int setPosition(float * pos);
		int setPowerOn(int power);
		int setSpeed(float * speed);
		int setSpeedFactor(float speed_factor);
		int setTexture(float * position, float * intensity, int reinit);
		int setTextureForce(float * texture_force);
		int setTimeStep(float step);
		int setTimeoutValue(float time_value);
		int startLoop();
		int stopLoop();
		int waitForSynch();
		int trajRecordStart();
		int trajRecordStop();
		int trajSetSamplingTimeStep(float timeStep, int unsigned * recordTime);
		int vmStartTrajSampling(int unsigned nbSamples);
		int vmGetTrajSamples(float * samples);
		int vmSetType(VirtVmType type);
		int vmSetParameter(VirtVmParameter * param);
		int vmActivate();
		int vmDeactivate();
		int vmSetBaseFrame(float * base);
		int vmSetMaxArtiBounds(float * bounds);
		int vmSetMinArtiBounds(float * bounds);
		int getPhysicalPosition(float * pos);
		int getAvatarPosition(float * pos);
		int saturateTorque(float forceThreshold, float momentThreshold);
		int vmSetDefaultToTransparentMode();
		int vmSetDefaultToCartesianPosition();
		int vmSetBaseFrameToCurrentFrame();
		int convertRGBToGrayscale(float * rgb, float * gray);
		int vmGetBaseFrame(float * base);
		int waitPressButton(int button_number);
		int getTimeStep(float * step);
		int vmSetRobotMode(int OnOff);
		int vmSaveCurrentSpline(char * file_name);
		int vmLoadSpline(char * file_name);
		int vmDeleteSpline(char * file_name);
		int vmWaitUpperBound();
		int disableControlConnexion(int disable);
		int isInBounds(int unsigned * bounds);
		int getAlarm(int unsigned * alarm);
		int getCatchFrame(float * frame);
		int setCatchFrame(float * frame);
		int activeSpeedControl(float radius, float speedFactor);
		int deactiveSpeedControl();
		int isInShiftPosition(int * shift);
		int setFrictionForce(float fx, float fy, float fz);
		int getMouseState(int * active, int * left_click, int * right_click);
		int generateDebugFile();
		int getCenterSphere(float * pos);
		int getAxisOfRotation(float * axis);
		int getADC(int line, float * adc);
		int convertDeplToHomogeneMatrix(float * d, float * m);
		int convertHomogeneMatrixToDepl(float * d, float * m);
		int getTrackball(int * x_move, int * y_move);
		int getTrackballButton(int * active, int * left_btn, int * middle_btn, int * right_btn);
		int setAbsolutePosition(float * pos);
		int beepOnLimit(int enable);
		int enableForceFeedback(int enable);
		int getPhysicalSpeed(float * speed);
		int forceShiftButton(int forceShiftButton);
		int addForce(float * force);
		int getFailure(int unsigned * error);
		int getArticularPositionOfAdditionalAxe(float * pos);
		int setArticularPositionOfAdditionalAxe(float * pos);
		int getArticularSpeedOfAdditionalAxe(float * speed);
		int setArticularSpeedOfAdditionalAxe(float * speed);
		int setArticularForceOfAdditionalAxe(float * effort);
		int getArticularPosition(float * pos);
		int setArticularPosition(float * pos);
		int getArticularSpeed(float * speed);
		int setArticularSpeed(float * speed);
		int setArticularForce(float * force);
		int activeRotationSpeedControl(float angle, float speedFactor);
		int deactiveRotationSpeedControl();
		int getControllerVersion(int * major, int * minor);
		int isInSpeedControl(int * translation, int * rotation);
		int setForceInSpeedControl(float force);
		int setTorqueInSpeedControl(float torque);
		/* Static Methods */
		static int APIVersion(int * major, int * minor);

		// Use like:
		// if (virt.checkForError(ret, __FILE__, __LINE__, __FUNCTION__)) {
		//   // handle error
		// }
		bool checkForError(int returnValue, const char * file = "",
		                   int const line = -1, const char * func = "") {
			if (returnValue == 0) {
				// Silence unused variable warning when VPP_VERBOSE not defined
				(void) file;
				(void) line;
				(void) func;
				return false; // no error
			} else {
				VPP_VERBOSE_MESSAGE("Got error from Virtuose (in " << func << "@" << file << ":" << line << "): "  << getErrorMessage());
				return true; // error
			}
		}

		std::string getErrorMessage() {
			return std::string(virtGetErrorMessage(virtGetErrorCode(_vc)));
		}

		static std::string getErrorMessage(int code) {
			return std::string(virtGetErrorMessage(code));
		}

	protected:
		std::string _name;
		VirtContext _vc;
		bool _weOpened;
};

/* Wrapper Implementation Details Follow */

inline int Virtuose::APIVersion(int * major, int * minor) {
	return virtAPIVersion(major, minor);
}

inline int Virtuose::attachVO(float mass, float * mxmymz) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtAttachVO(_vc, mass, mxmymz));
			return ret;
}

inline int Virtuose::attachQSVO(float * Ks, float * Bs) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtAttachQSVO(_vc, Ks, Bs));
			return ret;
}

inline int Virtuose::attachVOAvatar(float mass, float * mxmymz) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtAttachVOAvatar(_vc, mass, mxmymz));
			return ret;
}

inline int Virtuose::detachVO() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDetachVO(_vc));
			return ret;
}

inline int Virtuose::detachVOAvatar() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDetachVOAvatar(_vc));
			return ret;
}

inline int Virtuose::displayHardwareStatus(FILE * fh) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDisplayHardwareStatus(_vc, fh));
			return ret;
}

inline int Virtuose::getBaseFrame(float * base) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetBaseFrame(_vc, base));
			return ret;
}

inline int Virtuose::getButton(int button_number, int * state) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetButton(_vc, button_number, state));
			return ret;
}

inline int Virtuose::getCommandType(VirtCommandType * type) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetCommandType(_vc, type));
			return ret;
}

inline int Virtuose::getDeadMan(int * dead_man) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetDeadMan(_vc, dead_man));
			return ret;
}

inline int Virtuose::getEmergencyStop(int * emergency_stop) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetEmergencyStop(_vc, emergency_stop));
			return ret;
}

inline int Virtuose::getError(int * error) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetError(_vc, error));
			return ret;
}

inline int Virtuose::getErrorCode() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetErrorCode(_vc));
			return ret;
}

inline int Virtuose::getForce(float * force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetForce(_vc, force));
			return ret;
}

inline int Virtuose::getForceFactor(float * force_factor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetForceFactor(_vc, force_factor));
			return ret;
}

inline int Virtuose::getIndexingMode(VirtIndexingType * indexing_mode) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetIndexingMode(_vc, indexing_mode));
			return ret;
}

inline int Virtuose::getLimitTorque(float * torque) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetLimitTorque(_vc, torque));
			return ret;
}

inline int Virtuose::getObservationFrame(float * obs) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetObservationFrame(_vc, obs));
			return ret;
}

inline int Virtuose::getPosition(float * pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetPosition(_vc, pos));
			return ret;
}

inline int Virtuose::getPowerOn(int * power) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetPowerOn(_vc, power));
			return ret;
}

inline int Virtuose::getSpeed(float * speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetSpeed(_vc, speed));
			return ret;
}

inline int Virtuose::getSpeedFactor(float * speed_factor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetSpeedFactor(_vc, speed_factor));
			return ret;
}

inline int Virtuose::getTimeLastUpdate(int unsigned * time) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetTimeLastUpdate(_vc, time));
			return ret;
}

inline int Virtuose::getTimeoutValue(float * time_value) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetTimeoutValue(_vc, time_value));
			return ret;
}

inline int Virtuose::setBaseFrame(float * base) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetBaseFrame(_vc, base));
			return ret;
}

inline int Virtuose::setCommandType(VirtCommandType type) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetCommandType(_vc, type));
			return ret;
}

inline int Virtuose::setDebugFlags(short unsigned flag) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetDebugFlags(_vc, flag));
			return ret;
}

inline int Virtuose::setForce(float * force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetForce(_vc, force));
			return ret;
}

inline int Virtuose::setForceFactor(float force_factor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetForceFactor(_vc, force_factor));
			return ret;
}

inline int Virtuose::setIndexingMode(VirtIndexingType indexing_mode) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetIndexingMode(_vc, indexing_mode));
			return ret;
}

inline int Virtuose::setLimitTorque(float torque) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetLimitTorque(_vc, torque));
			return ret;
}

inline int Virtuose::setObservationFrame(float * obs) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetObservationFrame(_vc, obs));
			return ret;
}

inline int Virtuose::setObservationFrameSpeed(float * speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetObservationFrameSpeed(_vc, speed));
			return ret;
}

inline int Virtuose::setOutputFile(char * name) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetOutputFile(_vc, name));
			return ret;
}

inline int Virtuose::setPeriodicFunction(void (* fn ) (VirtContext, void *), float * period, void * arg) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetPeriodicFunction(_vc, fn, period, arg));
			return ret;
}

inline int Virtuose::setPosition(float * pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetPosition(_vc, pos));
			return ret;
}

inline int Virtuose::setPowerOn(int power) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetPowerOn(_vc, power));
			return ret;
}

inline int Virtuose::setSpeed(float * speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetSpeed(_vc, speed));
			return ret;
}

inline int Virtuose::setSpeedFactor(float speed_factor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetSpeedFactor(_vc, speed_factor));
			return ret;
}

inline int Virtuose::setTexture(float * position, float * intensity, int reinit) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetTexture(_vc, position, intensity, reinit));
			return ret;
}

inline int Virtuose::setTextureForce(float * texture_force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetTextureForce(_vc, texture_force));
			return ret;
}

inline int Virtuose::setTimeStep(float step) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetTimeStep(_vc, step));
			return ret;
}

inline int Virtuose::setTimeoutValue(float time_value) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetTimeoutValue(_vc, time_value));
			return ret;
}

inline int Virtuose::startLoop() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtStartLoop(_vc));
			return ret;
}

inline int Virtuose::stopLoop() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtStopLoop(_vc));
			return ret;
}

inline int Virtuose::waitForSynch() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtWaitForSynch(_vc));
			return ret;
}

inline int Virtuose::trajRecordStart() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtTrajRecordStart(_vc));
			return ret;
}

inline int Virtuose::trajRecordStop() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtTrajRecordStop(_vc));
			return ret;
}

inline int Virtuose::trajSetSamplingTimeStep(float timeStep, int unsigned * recordTime) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtTrajSetSamplingTimeStep(_vc, timeStep, recordTime));
			return ret;
}

inline int Virtuose::vmStartTrajSampling(int unsigned nbSamples) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmStartTrajSampling(_vc, nbSamples));
			return ret;
}

inline int Virtuose::vmGetTrajSamples(float * samples) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmGetTrajSamples(_vc, samples));
			return ret;
}

inline int Virtuose::vmSetType(VirtVmType type) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetType(_vc, type));
			return ret;
}

inline int Virtuose::vmSetParameter(VirtVmParameter * param) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetParameter(_vc, param));
			return ret;
}

inline int Virtuose::vmActivate() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmActivate(_vc));
			return ret;
}

inline int Virtuose::vmDeactivate() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmDeactivate(_vc));
			return ret;
}

inline int Virtuose::vmSetBaseFrame(float * base) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetBaseFrame(_vc, base));
			return ret;
}

inline int Virtuose::vmSetMaxArtiBounds(float * bounds) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetMaxArtiBounds(_vc, bounds));
			return ret;
}

inline int Virtuose::vmSetMinArtiBounds(float * bounds) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetMinArtiBounds(_vc, bounds));
			return ret;
}

inline int Virtuose::getPhysicalPosition(float * pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetPhysicalPosition(_vc, pos));
			return ret;
}

inline int Virtuose::getAvatarPosition(float * pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetAvatarPosition(_vc, pos));
			return ret;
}

inline int Virtuose::saturateTorque(float forceThreshold, float momentThreshold) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSaturateTorque(_vc, forceThreshold, momentThreshold));
			return ret;
}

inline int Virtuose::vmSetDefaultToTransparentMode() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetDefaultToTransparentMode(_vc));
			return ret;
}

inline int Virtuose::vmSetDefaultToCartesianPosition() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetDefaultToCartesianPosition(_vc));
			return ret;
}

inline int Virtuose::vmSetBaseFrameToCurrentFrame() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetBaseFrameToCurrentFrame(_vc));
			return ret;
}

inline int Virtuose::convertRGBToGrayscale(float * rgb, float * gray) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtConvertRGBToGrayscale(_vc, rgb, gray));
			return ret;
}

inline int Virtuose::vmGetBaseFrame(float * base) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmGetBaseFrame(_vc, base));
			return ret;
}

inline int Virtuose::waitPressButton(int button_number) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtWaitPressButton(_vc, button_number));
			return ret;
}

inline int Virtuose::getTimeStep(float * step) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetTimeStep(_vc, step));
			return ret;
}

inline int Virtuose::vmSetRobotMode(int OnOff) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetRobotMode(_vc, OnOff));
			return ret;
}

inline int Virtuose::vmSaveCurrentSpline(char * file_name) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSaveCurrentSpline(_vc, file_name));
			return ret;
}

inline int Virtuose::vmLoadSpline(char * file_name) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmLoadSpline(_vc, file_name));
			return ret;
}

inline int Virtuose::vmDeleteSpline(char * file_name) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmDeleteSpline(_vc, file_name));
			return ret;
}

inline int Virtuose::vmWaitUpperBound() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmWaitUpperBound(_vc));
			return ret;
}

inline int Virtuose::disableControlConnexion(int disable) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDisableControlConnexion(_vc, disable));
			return ret;
}

inline int Virtuose::isInBounds(int unsigned * bounds) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtIsInBounds(_vc, bounds));
			return ret;
}

inline int Virtuose::getAlarm(int unsigned * alarm) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetAlarm(_vc, alarm));
			return ret;
}

inline int Virtuose::getCatchFrame(float * frame) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetCatchFrame(_vc, frame));
			return ret;
}

inline int Virtuose::setCatchFrame(float * frame) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetCatchFrame(_vc, frame));
			return ret;
}

inline int Virtuose::activeSpeedControl(float radius, float speedFactor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtActiveSpeedControl(_vc, radius, speedFactor));
			return ret;
}

inline int Virtuose::deactiveSpeedControl() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDeactiveSpeedControl(_vc));
			return ret;
}

inline int Virtuose::isInShiftPosition(int * shift) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtIsInShiftPosition(_vc, shift));
			return ret;
}

inline int Virtuose::setFrictionForce(float fx, float fy, float fz) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetFrictionForce(_vc, fx, fy, fz));
			return ret;
}

inline int Virtuose::getMouseState(int * active, int * left_click, int * right_click) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetMouseState(_vc, active, left_click, right_click));
			return ret;
}

inline int Virtuose::generateDebugFile() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGenerateDebugFile(_vc));
			return ret;
}

inline int Virtuose::getCenterSphere(float * pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetCenterSphere(_vc, pos));
			return ret;
}

inline int Virtuose::getAxisOfRotation(float * axis) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetAxisOfRotation(_vc, axis));
			return ret;
}

inline int Virtuose::getADC(int line, float * adc) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetADC(_vc, line, adc));
			return ret;
}

inline int Virtuose::convertDeplToHomogeneMatrix(float * d, float * m) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtConvertDeplToHomogeneMatrix(_vc, d, m));
			return ret;
}

inline int Virtuose::convertHomogeneMatrixToDepl(float * d, float * m) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtConvertHomogeneMatrixToDepl(_vc, d, m));
			return ret;
}

inline int Virtuose::getTrackball(int * x_move, int * y_move) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetTrackball(_vc, x_move, y_move));
			return ret;
}

inline int Virtuose::getTrackballButton(int * active, int * left_btn, int * middle_btn, int * right_btn) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetTrackballButton(_vc, active, left_btn, middle_btn, right_btn));
			return ret;
}

inline int Virtuose::setAbsolutePosition(float * pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetAbsolutePosition(_vc, pos));
			return ret;
}

inline int Virtuose::beepOnLimit(int enable) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtBeepOnLimit(_vc, enable));
			return ret;
}

inline int Virtuose::enableForceFeedback(int enable) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtEnableForceFeedback(_vc, enable));
			return ret;
}

inline int Virtuose::getPhysicalSpeed(float * speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetPhysicalSpeed(_vc, speed));
			return ret;
}

inline int Virtuose::forceShiftButton(int forceShiftButton) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtForceShiftButton(_vc, forceShiftButton));
			return ret;
}

inline int Virtuose::addForce(float * force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtAddForce(_vc, force));
			return ret;
}

inline int Virtuose::getFailure(int unsigned * error) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetFailure(_vc, error));
			return ret;
}

inline int Virtuose::getArticularPositionOfAdditionalAxe(float * pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetArticularPositionOfAdditionalAxe(_vc, pos));
			return ret;
}

inline int Virtuose::setArticularPositionOfAdditionalAxe(float * pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularPositionOfAdditionalAxe(_vc, pos));
			return ret;
}

inline int Virtuose::getArticularSpeedOfAdditionalAxe(float * speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetArticularSpeedOfAdditionalAxe(_vc, speed));
			return ret;
}

inline int Virtuose::setArticularSpeedOfAdditionalAxe(float * speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularSpeedOfAdditionalAxe(_vc, speed));
			return ret;
}

inline int Virtuose::setArticularForceOfAdditionalAxe(float * effort) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularForceOfAdditionalAxe(_vc, effort));
			return ret;
}

inline int Virtuose::getArticularPosition(float * pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetArticularPosition(_vc, pos));
			return ret;
}

inline int Virtuose::setArticularPosition(float * pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularPosition(_vc, pos));
			return ret;
}

inline int Virtuose::getArticularSpeed(float * speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetArticularSpeed(_vc, speed));
			return ret;
}

inline int Virtuose::setArticularSpeed(float * speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularSpeed(_vc, speed));
			return ret;
}

inline int Virtuose::setArticularForce(float * force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularForce(_vc, force));
			return ret;
}

inline int Virtuose::activeRotationSpeedControl(float angle, float speedFactor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtActiveRotationSpeedControl(_vc, angle, speedFactor));
			return ret;
}

inline int Virtuose::deactiveRotationSpeedControl() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDeactiveRotationSpeedControl(_vc));
			return ret;
}

inline int Virtuose::getControllerVersion(int * major, int * minor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetControlerVersion(_vc, major, minor));
			return ret;
}

inline int Virtuose::isInSpeedControl(int * translation, int * rotation) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtIsInSpeedControl(_vc, translation, rotation));
			return ret;
}

inline int Virtuose::setForceInSpeedControl(float force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetForceInSpeedControl(_vc, force));
			return ret;
}

inline int Virtuose::setTorqueInSpeedControl(float torque) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetTorqueInSpeedControl(_vc, torque));
			return ret;
}

#undef VPP_VERBOSE_MESSAGE
#undef VPP_CHECKED_CALL

#endif // INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935

