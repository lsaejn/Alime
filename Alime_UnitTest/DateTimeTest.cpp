#include "easyTest.h"
#include <Alime/base/time/DateTime.h>
#include <Alime/base/time/TimeSpan.h>
#include <Alime/base/time/DateTime.cpp>


TEST_UNIT(DateTime_TEST)
{
	{
		int year = 2020;
		int month = 12;
		int day = 5;
		int hour = 8;

		int today_in_julian=Alime::base::System::getJulianDayNumber(year, month, day);
		int daysSpan = today_in_julian - Alime::base::System::kJulianDayOf1970_01_01;

		{
			time_t t = Alime::base::System::GetTimetFromDateInfo(year, month, day);
			tm* pTm = localtime(&t);
			ASSERT_EQ(pTm->tm_year + 1900, year);
			ASSERT_EQ(pTm->tm_mon + 1, month);
			ASSERT_EQ(pTm->tm_mday, day);
			ASSERT_EQ(pTm->tm_hour, 8);
			pTm = gmtime(&t);
			ASSERT_EQ(pTm->tm_year + 1900, year);
			ASSERT_EQ(pTm->tm_mon + 1, month);
			ASSERT_EQ(pTm->tm_mday, day);
		}

		{
			time_t t = Alime::base::System::GetTimetFromDateInfo(year, month, day, hour);
			tm* pTm = localtime(&t);
			//localtime_s
			ASSERT_EQ(pTm->tm_year + 1900, year);
			ASSERT_EQ(pTm->tm_mon + 1, month);
			ASSERT_EQ(pTm->tm_mday, day);
			ASSERT_EQ(pTm->tm_hour, hour+8);
			pTm = gmtime(&t);
			ASSERT_EQ(pTm->tm_year + 1900, year);
			ASSERT_EQ(pTm->tm_mon + 1, month);
			ASSERT_EQ(pTm->tm_mday, day);
		}

		{
			tm tm_t;
			tm_t = { 0 };
			tm_t.tm_year = year - 1900;
			tm_t.tm_mon = month - 1;
			tm_t.tm_mday = day;

			time_t t = Alime::base::System::GetTimetFromDateInfo(year, month, day);
			auto sec = _mkgmtime(&tm_t);
			ASSERT_EQ(t, sec);
			sec = mktime(&tm_t);
			ASSERT_EQ(t - 8 * 3600, sec);
		}
		{
			tm tm_t;
			tm_t = { 0 };
			tm_t.tm_year = year - 1900;
			tm_t.tm_mon = month - 1;
			tm_t.tm_mday = day;

			time_t t = Alime::base::System::GetTimetFromDateInfo(year, month, day);
			auto sec = _mkgmtime(&tm_t);
			ASSERT_EQ(t, sec);
			sec = mktime(&tm_t);
			ASSERT_EQ(t - 8 * 3600, sec);
		}
	}
	{
		Alime::base::System::DateTime d1(637428021492383024);
		ASSERT_EQ(d1.Day(), 5);
		ASSERT_EQ(d1.Hour(), 21);
		ASSERT_EQ(d1.Minute(), 55);
		ASSERT_EQ(d1.Second(), 49);
	}

	Alime::base::System::DateTime d(2020,12,5,8,59,32,223);
	auto ret=d.Year();
	ret=d.Month();
	ret=d.Day();
	ret=d.Minute();
	ret=d.Second();
	ret= d.DayOfWeek();
	ret = d.DayOfYear();
	ret = Alime::base::System::DateTime::DaysInMonth(d.Year(), d.Month());
	auto newDate = d.AddYears(2);
	auto newDate2 = d.AddMonths(33);

	bool flag = Alime::base::System::DateTime::IsLeapYear(d.Year());
	

}