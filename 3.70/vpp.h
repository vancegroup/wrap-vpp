/**
	@file
	@brief C++ wrapper for Haption's VirtuoseAPI

	Generated by parsing version 3.70 of
	virtuoseAPI.h

	@date
	2010-2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2010-2012.
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

/** @brief Preprocessor definition containing an integer version of the
	VirtuoseAPI this header was generated from, with the major version
	multiplied by 1000 and added to the minor version.
*/
#define VIRTUOSEAPI_VERSION 3070

/** @brief Pass arguments like 3, 60 to get a preprocessor expression
	that evaluates to true if the API used is at least that version.
*/
#define VIRTUOSEAPI_VERSION_CHECK(MAJOR, MINOR) (VIRTUOSEAPI_VERSION >= (MAJOR * 1000 + MINOR))

/** @brief Object representing a VirtuoseAPI device.

	It encapsulates and manages the lifetime of the VirtContext,
	and provides methods with names and signatures similar to the
	C API (generated from it, with minor automated modifications).

	This object is non-copyable and non-assignable: you may manage it
	with a scoped_ptr or shared_ptr and allocate it on the heap if
	those restrictions are a problem for you.
*/

class Virtuose {
	public:

#ifndef VPP_DISABLE_ERROR_CHECK
#	define VPP_CHECKED_CALL(_CALL) _checkReturnCode(_CALL, #_CALL, __FILE__, __LINE__, __FUNCTION__)
#else
#	define VPP_CHECKED_CALL(_CALL)
#endif

		struct VirtuoseAPIError : public std::runtime_error {
			VirtuoseAPIError(std::string const& what) : std::runtime_error(what) {}
		};

		/** @brief constructor

			Opens a new VirtContext upon construction.

			@param name Name of Virtuose device to connect to.

			@throws VirtuoseAPIError if opening the device failed.
		*/
		Virtuose(const std::string & name)
			: vc_(virtOpen(name_.c_str()))
			, name_(name)
			, own_(true) {
			VPP_VERBOSE_MESSAGE("Constructing a new Virtuose object, device named " << name_ << ", VirtContext=" << vc_);
			if (!vc_) {
				throw VirtuoseAPIError("Failed opening Virtuose " + name_ + ": " +  getErrorMessage());
			}
		}

		/** @brief constructor from existing VirtContext

			Does not open new VirtContext. Intended for use inside a
			periodic callback function.

			@param name Name of Virtuose device to connect to.

			@throws VirtuoseAPIError if opening the device failed.
		*/
		Virtuose(VirtContext vc)
			: vc_(vc)
			, name_("unknown - from VirtContext")
			, own_(false) {
			VPP_VERBOSE_MESSAGE("Borrowing a Virtuose object with VirtContext=" << vc_);
			if (!vc_) {
				throw VirtuoseAPIError("Can't borrow a null VirtContext!");
			}
		}

		/** @brief destructor that closes the connection to the Virtuose
			device if we opened it.
		*/
		~Virtuose();

		/** @brief Conversion operator to type VirtContext.
		*/
		operator VirtContext() {
			return vc_;
		}

		/** @brief Returns the managed VirtContext.
		*/
		VirtContext getVirtContext() const {
			return vc_;
		}

		/** @brief Retrieve name used to create device context, if available.
		*/
		std::string const& getName() const {
			return name_;
		}

		int attachVO(float mass, float *mxmymz);
		int attachQSVO(float *Ks, float *Bs);
		int attachVOAvatar(float mass, float *mxmymz);
		int detachVO();
		int detachVOAvatar();
		int displayHardwareStatus(FILE *fh);
		int getBaseFrame(float *base);
		int getButton(int button_number, int *state);
		int getCommandType(VirtCommandType *type);
		int getDeadMan(int *dead_man);
		int getEmergencyStop(int *emergency_stop);
		int getError(int *error);
		int getErrorCode();
		int getForce(float *force);
		int getForceFactor(float *force_factor);
		int getIndexingMode(VirtIndexingType *indexing_mode);
		int getLimitTorque(float *torque);
		int getObservationFrame(float *obs);
		int getPosition(float *pos);
		int getPowerOn(int *power);
		int getSpeed(float *speed);
		int getSpeedFactor(float *speed_factor);
		int getTimeLastUpdate(unsigned int *time);
		int getTimeoutValue(float *time_value);
		int setBaseFrame(float *base);
		int setCommandType(VirtCommandType type);
		int setDebugFlags(unsigned short flag);
		int setForce(float *force);
		int setForceFactor(float force_factor);
		int setIndexingMode(VirtIndexingType indexing_mode);
		int setLimitTorque(float torque);
		int setObservationFrame(float *obs);
		int setObservationFrameSpeed(float *speed);
		int setOutputFile(char *name);
		int setPeriodicFunction(void (*fn)(VirtContext, void *), float *period, void *arg);
		int setPosition(float *pos);
		int setPowerOn(int power);
		int setSpeed(float *speed);
		int setSpeedFactor(float speed_factor);
		int setTexture(float *position, float *intensity, int reinit);
		int setTextureForce(float *texture_force);
		int setTimeStep(float step);
		int setTimeoutValue(float time_value);
		int startLoop();
		int stopLoop();
		int waitForSynch();
		int trajRecordStart();
		int trajRecordStop();
		int trajSetSamplingTimeStep(float timeStep, unsigned int *recordTime);
		int vmStartTrajSampling(unsigned int nbSamples);
		int vmGetTrajSamples(float *samples);
		int vmSetType(VirtVmType type);
		int vmSetParameter(VirtVmParameter *param);
		int vmActivate();
		int vmDeactivate();
		int vmSetBaseFrame(float *base);
		int vmSetMaxArtiBounds(float *bounds);
		int vmSetMinArtiBounds(float *bounds);
		int getPhysicalPosition(float *pos);
		int getAvatarPosition(float *pos);
		int saturateTorque(float forceThreshold, float momentThreshold);
		int vmSetDefaultToTransparentMode();
		int vmSetDefaultToCartesianPosition();
		int vmSetBaseFrameToCurrentFrame();
		int convertRGBToGrayscale(float *rgb, float *gray);
		int vmGetBaseFrame(float *base);
		int waitPressButton(int button_number);
		int getTimeStep(float *step);
		int vmSetRobotMode(int OnOff);
		int vmSaveCurrentSpline(char *file_name);
		int vmLoadSpline(char *file_name);
		int vmDeleteSpline(char *file_name);
		int vmWaitUpperBound();
		int disableControlConnexion(int disable);
		int isInBounds(unsigned int *bounds);
		int getAlarm(unsigned int *alarm);
		int getCatchFrame(float *frame);
		int setCatchFrame(float *frame);
		int activeSpeedControl(float radius, float speedFactor);
		int deactiveSpeedControl();
		int isInShiftPosition(int *shift);
		int setFrictionForce(float fx, float fy, float fz);
		int getMouseState(int *active, int *left_click, int *right_click);
		int generateDebugFile();
		int getCenterSphere(float *pos);
		int getAxisOfRotation(float *axis);
		int getADC(int line, float *adc);
		int convertDeplToHomogeneMatrix(float *d, float *m);
		int convertHomogeneMatrixToDepl(float *d, float *m);
		int getTrackball(int *x_move, int *y_move);
		int getTrackballButton(int *active, int *left_btn, int *middle_btn, int *right_btn);
		int setAbsolutePosition(float *pos);
		int beepOnLimit(int enable);
		int enableForceFeedback(int enable);
		int getPhysicalSpeed(float *speed);
		int forceShiftButton(int forceShiftButton);
		int addForce(float *force);
		int getFailure(unsigned int *error);
		int getArticularPositionOfAdditionalAxe(float *pos);
		int setArticularPositionOfAdditionalAxe(float *pos);
		int getArticularSpeedOfAdditionalAxe(float *speed);
		int setArticularSpeedOfAdditionalAxe(float *speed);
		int setArticularForceOfAdditionalAxe(float *effort);
		int getArticularPosition(float *pos);
		int setArticularPosition(float *pos);
		int getArticularSpeed(float *speed);
		int setArticularSpeed(float *speed);
		int setArticularForce(float *force);
		int activeRotationSpeedControl(float angle, float speedFactor);
		int deactiveRotationSpeedControl();
		int getControllerVersion(int *major, int *minor);
		int isInSpeedControl(int *translation, int *rotation);
		int setForceInSpeedControl(float force);
		int setTorqueInSpeedControl(float torque);
		int setGripperCommandType(VirtGripperCommandType type);
		int outputsSetting(unsigned int outputs);
		/* Static Methods */
		static int APIVersion(int *major, int *minor);

		// Use like:
		// if (virt.checkForError(ret, __FILE__, __LINE__, __FUNCTION__)) {
		//   // handle error
		// }
		bool checkForError(int returnValue, const char * file = "",
		                   int const line = -1, const char * func = "");

		bool hasError();

		/** @brief Returns the latest error message, or if none, an empty string.
		*/
		std::string getErrorOrEmpty();

		/** @brief Returns the latest error message.
		*/
		std::string getErrorMessage();

		/** @brief Converts an error code into an error message string.
		*/
		static std::string getErrorMessage(int code);

	private:
		VirtContext vc_;
		std::string const name_;
		bool const own_;

		/// @brief Copy constructor forbidden
		Virtuose(Virtuose const&);

		/// @brief Assignment operator forbidden
		Virtuose & operator=(Virtuose const&);

#ifndef VPP_DISABLE_ERROR_CHECK
		void _checkReturnCode(int returnValue, const char * call, const char * file,
		                      int const line, const char * func = "") {
			if (returnValue != 0) {
				std::ostringstream s;
				s << "VirtuoseAPI Error (in call '" << call << "' in " << func << "@" << file << ":" << line << "): " << getErrorMessage();
				throw VirtuoseAPIError(s.str());
			}
		}
#endif
};

inline 	Virtuose::~Virtuose() {
	VPP_VERBOSE_MESSAGE("In destructor for device named " << name_ << ", VirtContext=" << vc_);
	if (vc_ && own_) {
		try {
			VPP_CHECKED_CALL(virtClose(vc_));
		} catch (VirtuoseAPIError & e) {
			(void) e; // silence warning if not in verbose mode.
			VPP_VERBOSE_MESSAGE("Exception in destructor, ignoring: " << e.what());
		}
	}
}

inline bool Virtuose::checkForError(int returnValue, const char * file,
                                    int const line, const char * func) {
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

inline bool Virtuose::hasError() {
	return (virtGetErrorCode(vc_) != VIRT_E_NO_ERROR);
}

inline std::string Virtuose::getErrorOrEmpty() {
	std::string ret;
	int code = virtGetErrorCode(vc_);
	if (virtGetErrorCode(vc_) != VIRT_E_NO_ERROR) {
		ret = getErrorMessage(code);
	}
	return ret;
}

/** @brief Returns the latest error message.
*/
inline std::string Virtuose::getErrorMessage() {
	return getErrorMessage(virtGetErrorCode(vc_));
}

/** @brief Converts an error code into an error message string.
*/
inline std::string Virtuose::getErrorMessage(int code) {
	return virtGetErrorMessage(code);
}

/// @brief Equality between a Virtuose object and a raw VirtContext.
inline bool operator==(Virtuose const& v, VirtContext const vc) {
	return (v.getVirtContext() == vc);
}

/// @brief Equality between a raw VirtContext and a Virtuose object.
inline bool operator==(VirtContext const vc, Virtuose const& v) {
	return (v.getVirtContext() == vc);
}

/// @brief Equality between two Virtuose objects - this should always be false!
inline bool operator==(Virtuose const& v1, Virtuose const& v2) {
	return (v1.getVirtContext() == v2.getVirtContext());
}

/// @brief Less-than comparison between Virtuose objects
inline bool operator<(Virtuose const& v1, Virtuose const& v2) {
	return (v1.getVirtContext() < v2.getVirtContext());
}

/* Wrapper Implementation Details Follow */

inline int Virtuose::APIVersion(int *major, int *minor) {
	return virtAPIVersion(major, minor);
}

inline int Virtuose::attachVO(float mass, float *mxmymz) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtAttachVO(vc_, mass, mxmymz));
			return ret;
}

inline int Virtuose::attachQSVO(float *Ks, float *Bs) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtAttachQSVO(vc_, Ks, Bs));
			return ret;
}

inline int Virtuose::attachVOAvatar(float mass, float *mxmymz) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtAttachVOAvatar(vc_, mass, mxmymz));
			return ret;
}

inline int Virtuose::detachVO() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDetachVO(vc_));
			return ret;
}

inline int Virtuose::detachVOAvatar() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDetachVOAvatar(vc_));
			return ret;
}

inline int Virtuose::displayHardwareStatus(FILE *fh) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDisplayHardwareStatus(vc_, fh));
			return ret;
}

inline int Virtuose::getBaseFrame(float *base) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetBaseFrame(vc_, base));
			return ret;
}

inline int Virtuose::getButton(int button_number, int *state) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetButton(vc_, button_number, state));
			return ret;
}

inline int Virtuose::getCommandType(VirtCommandType *type) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetCommandType(vc_, type));
			return ret;
}

inline int Virtuose::getDeadMan(int *dead_man) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetDeadMan(vc_, dead_man));
			return ret;
}

inline int Virtuose::getEmergencyStop(int *emergency_stop) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetEmergencyStop(vc_, emergency_stop));
			return ret;
}

inline int Virtuose::getError(int *error) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetError(vc_, error));
			return ret;
}

inline int Virtuose::getErrorCode() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetErrorCode(vc_));
			return ret;
}

inline int Virtuose::getForce(float *force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetForce(vc_, force));
			return ret;
}

inline int Virtuose::getForceFactor(float *force_factor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetForceFactor(vc_, force_factor));
			return ret;
}

inline int Virtuose::getIndexingMode(VirtIndexingType *indexing_mode) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetIndexingMode(vc_, indexing_mode));
			return ret;
}

inline int Virtuose::getLimitTorque(float *torque) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetLimitTorque(vc_, torque));
			return ret;
}

inline int Virtuose::getObservationFrame(float *obs) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetObservationFrame(vc_, obs));
			return ret;
}

inline int Virtuose::getPosition(float *pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetPosition(vc_, pos));
			return ret;
}

inline int Virtuose::getPowerOn(int *power) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetPowerOn(vc_, power));
			return ret;
}

inline int Virtuose::getSpeed(float *speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetSpeed(vc_, speed));
			return ret;
}

inline int Virtuose::getSpeedFactor(float *speed_factor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetSpeedFactor(vc_, speed_factor));
			return ret;
}

inline int Virtuose::getTimeLastUpdate(unsigned int *time) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetTimeLastUpdate(vc_, time));
			return ret;
}

inline int Virtuose::getTimeoutValue(float *time_value) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetTimeoutValue(vc_, time_value));
			return ret;
}

inline int Virtuose::setBaseFrame(float *base) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetBaseFrame(vc_, base));
			return ret;
}

inline int Virtuose::setCommandType(VirtCommandType type) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetCommandType(vc_, type));
			return ret;
}

inline int Virtuose::setDebugFlags(unsigned short flag) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetDebugFlags(vc_, flag));
			return ret;
}

inline int Virtuose::setForce(float *force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetForce(vc_, force));
			return ret;
}

inline int Virtuose::setForceFactor(float force_factor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetForceFactor(vc_, force_factor));
			return ret;
}

inline int Virtuose::setIndexingMode(VirtIndexingType indexing_mode) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetIndexingMode(vc_, indexing_mode));
			return ret;
}

inline int Virtuose::setLimitTorque(float torque) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetLimitTorque(vc_, torque));
			return ret;
}

inline int Virtuose::setObservationFrame(float *obs) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetObservationFrame(vc_, obs));
			return ret;
}

inline int Virtuose::setObservationFrameSpeed(float *speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetObservationFrameSpeed(vc_, speed));
			return ret;
}

inline int Virtuose::setOutputFile(char *name) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetOutputFile(vc_, name));
			return ret;
}

inline int Virtuose::setPeriodicFunction(void (*fn)(VirtContext, void *), float *period, void *arg) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetPeriodicFunction(vc_, fn, period, arg));
			return ret;
}

inline int Virtuose::setPosition(float *pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetPosition(vc_, pos));
			return ret;
}

inline int Virtuose::setPowerOn(int power) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetPowerOn(vc_, power));
			return ret;
}

inline int Virtuose::setSpeed(float *speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetSpeed(vc_, speed));
			return ret;
}

inline int Virtuose::setSpeedFactor(float speed_factor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetSpeedFactor(vc_, speed_factor));
			return ret;
}

inline int Virtuose::setTexture(float *position, float *intensity, int reinit) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetTexture(vc_, position, intensity, reinit));
			return ret;
}

inline int Virtuose::setTextureForce(float *texture_force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetTextureForce(vc_, texture_force));
			return ret;
}

inline int Virtuose::setTimeStep(float step) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetTimeStep(vc_, step));
			return ret;
}

inline int Virtuose::setTimeoutValue(float time_value) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetTimeoutValue(vc_, time_value));
			return ret;
}

inline int Virtuose::startLoop() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtStartLoop(vc_));
			return ret;
}

inline int Virtuose::stopLoop() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtStopLoop(vc_));
			return ret;
}

inline int Virtuose::waitForSynch() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtWaitForSynch(vc_));
			return ret;
}

inline int Virtuose::trajRecordStart() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtTrajRecordStart(vc_));
			return ret;
}

inline int Virtuose::trajRecordStop() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtTrajRecordStop(vc_));
			return ret;
}

inline int Virtuose::trajSetSamplingTimeStep(float timeStep, unsigned int *recordTime) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtTrajSetSamplingTimeStep(vc_, timeStep, recordTime));
			return ret;
}

inline int Virtuose::vmStartTrajSampling(unsigned int nbSamples) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmStartTrajSampling(vc_, nbSamples));
			return ret;
}

inline int Virtuose::vmGetTrajSamples(float *samples) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmGetTrajSamples(vc_, samples));
			return ret;
}

inline int Virtuose::vmSetType(VirtVmType type) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetType(vc_, type));
			return ret;
}

inline int Virtuose::vmSetParameter(VirtVmParameter *param) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetParameter(vc_, param));
			return ret;
}

inline int Virtuose::vmActivate() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmActivate(vc_));
			return ret;
}

inline int Virtuose::vmDeactivate() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmDeactivate(vc_));
			return ret;
}

inline int Virtuose::vmSetBaseFrame(float *base) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetBaseFrame(vc_, base));
			return ret;
}

inline int Virtuose::vmSetMaxArtiBounds(float *bounds) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetMaxArtiBounds(vc_, bounds));
			return ret;
}

inline int Virtuose::vmSetMinArtiBounds(float *bounds) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetMinArtiBounds(vc_, bounds));
			return ret;
}

inline int Virtuose::getPhysicalPosition(float *pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetPhysicalPosition(vc_, pos));
			return ret;
}

inline int Virtuose::getAvatarPosition(float *pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetAvatarPosition(vc_, pos));
			return ret;
}

inline int Virtuose::saturateTorque(float forceThreshold, float momentThreshold) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSaturateTorque(vc_, forceThreshold, momentThreshold));
			return ret;
}

inline int Virtuose::vmSetDefaultToTransparentMode() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetDefaultToTransparentMode(vc_));
			return ret;
}

inline int Virtuose::vmSetDefaultToCartesianPosition() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetDefaultToCartesianPosition(vc_));
			return ret;
}

inline int Virtuose::vmSetBaseFrameToCurrentFrame() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetBaseFrameToCurrentFrame(vc_));
			return ret;
}

inline int Virtuose::convertRGBToGrayscale(float *rgb, float *gray) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtConvertRGBToGrayscale(vc_, rgb, gray));
			return ret;
}

inline int Virtuose::vmGetBaseFrame(float *base) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmGetBaseFrame(vc_, base));
			return ret;
}

inline int Virtuose::waitPressButton(int button_number) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtWaitPressButton(vc_, button_number));
			return ret;
}

inline int Virtuose::getTimeStep(float *step) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetTimeStep(vc_, step));
			return ret;
}

inline int Virtuose::vmSetRobotMode(int OnOff) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSetRobotMode(vc_, OnOff));
			return ret;
}

inline int Virtuose::vmSaveCurrentSpline(char *file_name) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmSaveCurrentSpline(vc_, file_name));
			return ret;
}

inline int Virtuose::vmLoadSpline(char *file_name) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmLoadSpline(vc_, file_name));
			return ret;
}

inline int Virtuose::vmDeleteSpline(char *file_name) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmDeleteSpline(vc_, file_name));
			return ret;
}

inline int Virtuose::vmWaitUpperBound() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtVmWaitUpperBound(vc_));
			return ret;
}

inline int Virtuose::disableControlConnexion(int disable) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDisableControlConnexion(vc_, disable));
			return ret;
}

inline int Virtuose::isInBounds(unsigned int *bounds) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtIsInBounds(vc_, bounds));
			return ret;
}

inline int Virtuose::getAlarm(unsigned int *alarm) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetAlarm(vc_, alarm));
			return ret;
}

inline int Virtuose::getCatchFrame(float *frame) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetCatchFrame(vc_, frame));
			return ret;
}

inline int Virtuose::setCatchFrame(float *frame) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetCatchFrame(vc_, frame));
			return ret;
}

inline int Virtuose::activeSpeedControl(float radius, float speedFactor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtActiveSpeedControl(vc_, radius, speedFactor));
			return ret;
}

inline int Virtuose::deactiveSpeedControl() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDeactiveSpeedControl(vc_));
			return ret;
}

inline int Virtuose::isInShiftPosition(int *shift) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtIsInShiftPosition(vc_, shift));
			return ret;
}

inline int Virtuose::setFrictionForce(float fx, float fy, float fz) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetFrictionForce(vc_, fx, fy, fz));
			return ret;
}

inline int Virtuose::getMouseState(int *active, int *left_click, int *right_click) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetMouseState(vc_, active, left_click, right_click));
			return ret;
}

inline int Virtuose::generateDebugFile() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGenerateDebugFile(vc_));
			return ret;
}

inline int Virtuose::getCenterSphere(float *pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetCenterSphere(vc_, pos));
			return ret;
}

inline int Virtuose::getAxisOfRotation(float *axis) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetAxisOfRotation(vc_, axis));
			return ret;
}

inline int Virtuose::getADC(int line, float *adc) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetADC(vc_, line, adc));
			return ret;
}

inline int Virtuose::convertDeplToHomogeneMatrix(float *d, float *m) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtConvertDeplToHomogeneMatrix(vc_, d, m));
			return ret;
}

inline int Virtuose::convertHomogeneMatrixToDepl(float *d, float *m) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtConvertHomogeneMatrixToDepl(vc_, d, m));
			return ret;
}

inline int Virtuose::getTrackball(int *x_move, int *y_move) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetTrackball(vc_, x_move, y_move));
			return ret;
}

inline int Virtuose::getTrackballButton(int *active, int *left_btn, int *middle_btn, int *right_btn) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetTrackballButton(vc_, active, left_btn, middle_btn, right_btn));
			return ret;
}

inline int Virtuose::setAbsolutePosition(float *pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetAbsolutePosition(vc_, pos));
			return ret;
}

inline int Virtuose::beepOnLimit(int enable) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtBeepOnLimit(vc_, enable));
			return ret;
}

inline int Virtuose::enableForceFeedback(int enable) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtEnableForceFeedback(vc_, enable));
			return ret;
}

inline int Virtuose::getPhysicalSpeed(float *speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetPhysicalSpeed(vc_, speed));
			return ret;
}

inline int Virtuose::forceShiftButton(int forceShiftButton) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtForceShiftButton(vc_, forceShiftButton));
			return ret;
}

inline int Virtuose::addForce(float *force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtAddForce(vc_, force));
			return ret;
}

inline int Virtuose::getFailure(unsigned int *error) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetFailure(vc_, error));
			return ret;
}

inline int Virtuose::getArticularPositionOfAdditionalAxe(float *pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetArticularPositionOfAdditionalAxe(vc_, pos));
			return ret;
}

inline int Virtuose::setArticularPositionOfAdditionalAxe(float *pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularPositionOfAdditionalAxe(vc_, pos));
			return ret;
}

inline int Virtuose::getArticularSpeedOfAdditionalAxe(float *speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetArticularSpeedOfAdditionalAxe(vc_, speed));
			return ret;
}

inline int Virtuose::setArticularSpeedOfAdditionalAxe(float *speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularSpeedOfAdditionalAxe(vc_, speed));
			return ret;
}

inline int Virtuose::setArticularForceOfAdditionalAxe(float *effort) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularForceOfAdditionalAxe(vc_, effort));
			return ret;
}

inline int Virtuose::getArticularPosition(float *pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetArticularPosition(vc_, pos));
			return ret;
}

inline int Virtuose::setArticularPosition(float *pos) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularPosition(vc_, pos));
			return ret;
}

inline int Virtuose::getArticularSpeed(float *speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetArticularSpeed(vc_, speed));
			return ret;
}

inline int Virtuose::setArticularSpeed(float *speed) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularSpeed(vc_, speed));
			return ret;
}

inline int Virtuose::setArticularForce(float *force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetArticularForce(vc_, force));
			return ret;
}

inline int Virtuose::activeRotationSpeedControl(float angle, float speedFactor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtActiveRotationSpeedControl(vc_, angle, speedFactor));
			return ret;
}

inline int Virtuose::deactiveRotationSpeedControl() {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtDeactiveRotationSpeedControl(vc_));
			return ret;
}

inline int Virtuose::getControllerVersion(int *major, int *minor) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtGetControlerVersion(vc_, major, minor));
			return ret;
}

inline int Virtuose::isInSpeedControl(int *translation, int *rotation) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtIsInSpeedControl(vc_, translation, rotation));
			return ret;
}

inline int Virtuose::setForceInSpeedControl(float force) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetForceInSpeedControl(vc_, force));
			return ret;
}

inline int Virtuose::setTorqueInSpeedControl(float torque) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetTorqueInSpeedControl(vc_, torque));
			return ret;
}

inline int Virtuose::setGripperCommandType(VirtGripperCommandType type) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtSetGripperCommandType(vc_, type));
			return ret;
}

inline int Virtuose::outputsSetting(unsigned int outputs) {
	
			int ret;
			VPP_CHECKED_CALL(ret = virtOutputsSetting(vc_, outputs));
			return ret;
}

#undef VPP_VERBOSE_MESSAGE
#undef VPP_CHECKED_CALL

#endif // INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935

