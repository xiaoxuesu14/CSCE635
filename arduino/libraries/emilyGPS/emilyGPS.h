#ifndef __EMILYGPS_H_DEFINED__
#define __EMILYGPS_H_DEFINED__

#include <stdint.h>
#include "emilyStatus.h"

/** MAximum NMEA string sentence length */
const int sentenceSize = 80;
/** @file */


class emilyGPS{
public:
  /** Constructor
   *
   * @param[in] st pointer to the global status object
   */
  emilyGPS(emilyStatus*st);
  void parseBytes(char ch);/** Parse a sequence of raw bytes from serial port */
  void parseSentence();/** Parse a sentence from the GPS */
  void misc_tasks();/** Placeholder for miscellaneous tasks */
private:
  char sentence[sentenceSize];/** Buffer that holds incoming strings */
  emilyStatus* status; /** Pointer to the global status object */
  int parseCounter; /** Counts the position within the buffer */
  void getField(char* buffer, int index);/** Read a field from sentence */
};

/** Convert a GPS character field for longitude or latitude in standard format to a int32_t format in (10^-7 degrees)
 * Standard format is: xxxyy.zz, where xxx is whole degrees, and yy.zz is minutes. Maximum percent error is about 0.0001 ... in degrees
 */
int32_t convertGPS(char* buffer);
/** Convert GPS time field to a UTC time in seconds (INT) since midnight. Rolls over at midnight UTC! */
int32_t convertTime(char*buffer);
/** Convert GPS time field to a UTC time in seconds (FLOAT) since midnight. Rolls over at midnight UTC! */
float convertTimef(char*buffer);

#endif
