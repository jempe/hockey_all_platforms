//
//  CCLocalizedString.h
//

#ifndef _CCLocalizedString_h
#define _CCLocalizedString_h

/**get the localized string by the key, if can't get the value then return mComment
 support for file name below:
 "language_en";//english
 "language_es";//spanish
 */
const char * CCLocalizedString(const char * mKey);

/**purge localize string cached
 */
void PurgeCCLocalizedStringCached();

#endif
