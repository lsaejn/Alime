#include "easyTest.h"
#include <Alime/base/time/DateTime.h>
#include <Alime/base/time/TimeSpan.h>
#include <Alime/base/time/DateTime.cpp>
#include <Alime/base/fileSystem/file.h>

using namespace Alime::base::System;

TEST_UNIT(DateTime_Kind)
{
	DateTime t = DateTime::Now();
	aint64 tick = t.Ticks();
	DateTime t2 = DateTime::UtcNow();
	aint64 tick2= t2.Ticks();
	DateTime t3(t.Ticks());
	aint64 tick3 = t3.Ticks();
	DateTime t4(t.Ticks());
	aint64 tick4 = t4.Ticks();
}

TEST_UNIT(DateTime_FileTime)
{
	DateTime dt(2020, 1, 1);
	auto dow = dt.DayOfYear();

	auto filename = L"DateTimeTest.cpp";
	auto ct = IO::File::GetCreationTime(filename);
	auto tt = IO::File::GetCreationTimeUtc(filename);
	auto datoftheyear = tt.DayOfYear();
	auto datoftheweek = tt.DayInWeek();
	auto cut = IO::File::GetLastWriteTime(filename);
	auto cut2 = IO::File::GetLastWriteTimeUtc(filename);
	auto c3 = IO::File::GetLastAccessTime(filename);
	auto c4 = IO::File::GetLastAccessTimeUtc(filename);
	int x3 = 3;
}

TEST_UNIT(DateTime_TEST_ToString)
{
	//test toString series
	{
		DateTime t(2022, 2, 3, 3, 22, 43, 223);
		String localDateStr = t.ToLongDateString();
		String lts = t.ToLongTimeString();
		String st = t.ToShortDateString();
		String sts = t.ToShortTimeString();
		String ts = t.ToString();
		t=t.AddDays(1);
	}

	//test add sub
	{
		DateTime date(2021, 2, 28, 16, 30);
		ASSERT_FALSE(DateTime::IsLeapYear(date.Year()));
		date=date.AddDays(0);
		ASSERT_TRUE(date.Year() == 2021);
		ASSERT_TRUE(date.Month() == 2);
		ASSERT_TRUE(date.Day() == 28);
		date=date.AddDays(1);
		ASSERT_TRUE(date.Year() == 2021);
		ASSERT_TRUE(date.Month() == 3);
		ASSERT_TRUE(date.Day() == 1);
		date=date.AddDays(1);
		ASSERT_TRUE(date.Year() == 2021);
		ASSERT_TRUE(date.Month() == 3);
		ASSERT_TRUE(date.Day() == 2);
		date = date.AddDays(-2);
		ASSERT_TRUE(date.Year() == 2021);
		ASSERT_TRUE(date.Month() == 2);
		ASSERT_TRUE(date.Day() == 28);

		ASSERT_TRUE(date.Year() == 2021);
		date=date.AddYears(0);
		ASSERT_TRUE(date.Year() == 2021);
		ASSERT_TRUE(!DateTime::IsLeapYear(date.Year()));
		ASSERT_TRUE(date.Month() == 2);
		ASSERT_TRUE(date.Day() == 28);
		date = date.AddDays(1);
		ASSERT_TRUE(date.Year() == 2021);
		ASSERT_FALSE(date.Month() == 2);
		ASSERT_FALSE(date.Day() == 29);
		ASSERT_TRUE(date.Year() == 2021);
		ASSERT_TRUE(date.Month() == 3);
		ASSERT_TRUE(date.Day() == 1);

		ASSERT_TRUE(date.AddYears(0).Year() == 2021);
		ASSERT_TRUE(date.AddYears(1).Year() == 2022);
		ASSERT_TRUE(date.AddYears(2).Year() == 2023);
		ASSERT_TRUE(date.AddYears(3).Year() == 2024);
		ASSERT_TRUE(date.AddYears(4).Year() == 2025);

		bool ret = DateTime::IsLeapYear(date.Year());
		ret = DateTime::IsLeapYear(date.AddYears(1).Year());
		ret = DateTime::IsLeapYear(date.AddYears(2).Year());
		ret = DateTime::IsLeapYear(date.AddYears(3).Year());
		ret = DateTime::IsLeapYear(date.AddYears(4).Year());
		ret = DateTime::IsLeapYear(date.AddYears(5).Year());
	}
	{
		Alime::base::System::DateTime dtm = Alime::base::System::DateTime::Now();
		Alime::base::System::DateTime dtm2 = Alime::base::System::DateTime::UtcNow();

		auto Str=dtm.ToLongTimeString();
		auto utc=dtm.ToUniversalTime();

		
		auto Str2 = dtm2.ToLongTimeString();

	}

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
	ret= (int)d.DayInWeek();
	ret = d.DayOfYear();
	ret = Alime::base::System::DateTime::DaysInMonth(d.Year(), d.Month());
	auto newDate = d.AddYears(2);
	auto newDate2 = d.AddMonths(33);

	bool flag = Alime::base::System::DateTime::IsLeapYear(d.Year());
	
}

TEST_UNIT(DateTime_TEST_MilliSecond)
{
	//test MilliSecond
	{
		DateTime d(2020, 12, 8, 10, 38, 22, 320);
		auto re = d.Millisecond();
		ASSERT_TRUE(re == 320);
	}
}

TEST_UNIT(DateTime_TEST_IsLeapYear)
{
	{
		EXPECT_ANY_THROW(
			{
				bool ret = DateTime::IsLeapYear(-4);
			}
		);
		EXPECT_ANY_THROW(
			{
				DateTime aDay(1900, 2, 29);
			}
		);
		ASSERT_TRUE(DateTime::IsLeapYear(0));
		ASSERT_TRUE(DateTime::IsLeapYear(4));
		ASSERT_TRUE(DateTime::IsLeapYear(2020));
		ASSERT_TRUE(!DateTime::IsLeapYear(2021));
		ASSERT_TRUE(!DateTime::IsLeapYear(2022));
		ASSERT_TRUE(!DateTime::IsLeapYear(2023));
		ASSERT_TRUE(DateTime::IsLeapYear(2024));
		ASSERT_TRUE(!DateTime::IsLeapYear(2025));
		ASSERT_TRUE(!DateTime::IsLeapYear(2026));

		ASSERT_FALSE(DateTime::IsLeapYear(1900));
		int num = getJulianDayNumber(1900, 2, 29);
		ASSERT_TRUE(num == 2415080);
		
	}
}

TEST_UNIT(DateTime_TEST_Days)
{
	{
		//is 2012/01/01 a Sunday?
		DateTime d(2012, 1, 1);
		auto dow = d.DayInWeek();
		int doy = d.DayOfYear();
		//When is the 1000th after 2009/12/25
		DateTime d2(2009, 12, 25);
		auto strTime = d2.ToLongTimeString();
		d2 = d2.AddDays(1000);
		ASSERT_TRUE(d2.Year() == 2012);
		ASSERT_TRUE(d2.Month() == 9);
		ASSERT_TRUE(d2.Day() == 20);
	}
}

TEST_UNIT(DateTime_TEST_JulianDayNumber)
{
		const int kDaysInMonth[2][12] = {
		 { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
		 { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
		};

		int juliaDayNumber = 2415021;
		int weekDay = 1;
		ASSERT_TRUE(juliaDayNumber == getJulianDayNumber(1900, 1, 1));
		DateTime d(1900, 1, 1);

		ASSERT_TRUE(d.Year() == 1900);
		ASSERT_TRUE(d.Month() == 1);
		ASSERT_TRUE(d.Day() == 1);
		for (int year = 1900; year < 2500; ++year)
		{
			for (int month = 1; month < 12; ++month)
			{
				for (int day = 1; day < kDaysInMonth[DateTime::IsLeapYear(year)][month-1]; ++day)
				{
					if (day == 28)
					{
						int stop = 1;
					}
					DateTime temp(year, month, day);
					auto ymd = getYearMonthDayFromTicks(temp.Ticks());
					auto ymd2 = getYearMonthDay(juliaDayNumber);
					ASSERT_TRUE(ymd.year == year);
					if (ymd.month != month)
					{
						getchar();
					}
					ASSERT_TRUE(ymd.month == month);
					ASSERT_TRUE(ymd.day == day);
					//ASSERT_TRUE((int)temp.DayInWeek() == weekDay);

					++juliaDayNumber;
					weekDay = (weekDay + 1) % 7;
				}
			}
		}
}

TEST_UNIT(DateTime_TEST_NowSeries)
{
	{
		DateTime date = Alime::base::System::DateTime::Now();
		auto strTime = date.ToLongTimeString();

		date = Alime::base::System::DateTime::UtcNow();
		strTime = date.ToLongTimeString();

		DateTime date2(2020, 12, 7);
		strTime = date2.ToLongTimeString();

		auto today = DateTime::Today();
		today.ToLongTimeString();
		time_t tt = time(NULL);
		tm* tm_t = localtime(&tt);

		ASSERT_TRUE(today.Year() == tm_t->tm_year + 1900);
		ASSERT_TRUE(today.Month() == tm_t->tm_mon + 1);
		ASSERT_TRUE(today.Day() == tm_t->tm_mday);
		ASSERT_TRUE(today.Hour() == 0);
		ASSERT_TRUE(today.Minute() == 0);
		ASSERT_TRUE(today.Second() == 0);
		ASSERT_TRUE(today.Millisecond() == 0);
	}
	{
		//test local now
		DateTime date = Alime::base::System::DateTime::Now();
		auto Str = date.ToLongTimeString();

		time_t tt = time(NULL);
		tm* tm_t = localtime(&tt);
		ASSERT_TRUE(tm_t->tm_year + 1900 == date.Year());
		ASSERT_TRUE(tm_t->tm_mon + 1 == date.Month());
		ASSERT_TRUE(tm_t->tm_mday == date.Day());
		ASSERT_TRUE(tm_t->tm_hour == date.Hour());
		ASSERT_TRUE(tm_t->tm_min == date.Minute());
		ASSERT_TRUE(tm_t->tm_sec == date.Second());
	}
	{
		//test utc now
		DateTime date = Alime::base::System::DateTime::UtcNow();
		auto Str = date.ToLongTimeString();

		time_t tt = time(NULL);
		tm* tm_t = gmtime(&tt);
		ASSERT_TRUE(tm_t->tm_year + 1900 == date.Year());
		ASSERT_TRUE(tm_t->tm_mon + 1 == date.Month());
		ASSERT_TRUE(tm_t->tm_mday == date.Day());
		ASSERT_TRUE(tm_t->tm_hour == date.Hour());
		ASSERT_TRUE(tm_t->tm_min == date.Minute());
		ASSERT_TRUE(tm_t->tm_sec == date.Second());
	}
}