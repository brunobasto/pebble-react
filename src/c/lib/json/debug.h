/*
 * MIT License http://dogan.mit-license.org/
 */

 #pragma once

// Use #define DEBUG_ON to enable debugs
// Use #define DEBUG_DEEP to enable deep debugs (on almost each call)

#ifdef DEBUG_DEEP

#ifndef DEBUG_ON
#define DEBUG_ON
#define DEEP_DEBUG_CODE(code) code
#endif

#else 
#define DEEP_DEBUG_CODE(code)
#endif

#ifdef DEBUG_ON

#define DEBUG_CODE(code) code
#define LOG_INFO(message, ...) APP_LOG(APP_LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#define LOG_DEBUG(message, ...) APP_LOG(APP_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...) APP_LOG(APP_LOG_LEVEL_ERROR, message, ##__VA_ARGS__)

#else

#define DEBUG_CODE(code)
#define LOG_INFO(message, ...) 
#define LOG_DEBUG(message, ...) 
#define LOG_DEBUG(message, ...) 

#endif