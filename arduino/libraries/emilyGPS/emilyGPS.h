#ifndef __EMILYGPS_H_DEFINED__
#define __EMILYGPS_H_DEFINED__

#include <stdint.h>
#include "emilyStatus.h"

const int sentenceSize = 80;

class emilyGPS{
public:
  /** Constructor
   *
   * @param[in] st pointer to the global status object
   */
  emilyGPS(emilyStatus*st);
  void parseBytes(char ch);
  void parseSentence();/** Parse a sentence from the GPS */
  void misc_tasks();
private:
  char sentence[sentenceSize];
  emilyStatus* status;
  int parseCounter;
  void getField(char* buffer, int index);/** Read a field from sentence */
};

/** Convert a GPS character field for longitude or latitude in standard format to a int32_t format in (10^-7 degrees)
 * Standard format is: xxxyy.zz, where xxx is whole degrees, and yy.zz is minutes
 */
int32_t convertGPS(char* buffer);

#endif
