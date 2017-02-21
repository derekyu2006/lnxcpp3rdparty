#include <iostream>

#include "ccommon/time_cost/time_cost.h"

int main() {
  boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
  boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
  boost::posix_time::time_duration t = now - epoch;
  int64_t ut = t.total_milliseconds() / 1000;
  std::cout << "universal time stamp is: " << ut << std::endl;

  int64_t nt = newstone::ccommon::NowUTCTick();
  std::cout << "NowUTCTick is:" << nt << std::endl;
  std::cout << "local tick is:" << newstone::ccommon::CurUTCTimeStampSeconds() << std::endl;
  std::string str1;

  newstone::ccommon::TimestampToTime(str1, nt);
  std::cout << "utc time is:" << str1 << std::endl;

  int64_t bt = newstone::ccommon::UTCToZoneSeconds(nt, 8);
  std::cout << "utc seconds " << nt << "--" << "beijing seconds:" << bt << std::endl;
  std::string str2;
  newstone::ccommon::TimestampToTime(str2, bt);
  std::cout << "beijing time is:" << str2 << std::endl;

  int64_t ct = newstone::ccommon::TimeToTimestamp(str2);
  int64_t uut = newstone::ccommon::ZoneToUTCSeconds(ct, 8);
  std::cout << "beijing seconds:" << ct << "--" << "utc seconds:" << uut << std::endl;
  std::string str3;
  newstone::ccommon::TimestampToTime(str3, ct);
  std::cout << "beijing time::" << str3 << std::endl;
  std::string str4;
  newstone::ccommon::TimestampToTime(str4, uut);
  std::cout << "utc time:" << str4 << std::endl;

  std::string date_str = "20151023 13:24:23.001";
  int64_t millisecs = newstone::ccommon::DateStringToUTCTimeStamp(date_str);
  std::cout << "utc timestamp of date[" << date_str << "] is [" << millisecs
    << "]." << std::endl;


  int64_t ts_01 = newstone::ccommon::TimeToTimestamp(date_str);
  std::cout << "utc timestamp of date[" << date_str << "] is [" << ts_01
    << "]." << std::endl;



  return 0;
}

