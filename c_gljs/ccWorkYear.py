import sys
import math
import datetime

def delteYearAfter08(dateSt, dateEd):
    #print(dateSt.year,dateEd.year)
    year_floor = dateEd.year - dateSt.year
    #print(year_floor)
    datebuf = datetime.datetime.strptime(
        '%d-%d-%d'%(dateEd.year,dateSt.month,dateSt.day),'%Y-%m-%d')
    day1 = datetime.datetime.strptime('%d-1-1'%(dateEd.year),'%Y-%m-%d')
    day2 = datetime.datetime.strptime('%d-12-31'%(dateEd.year),'%Y-%m-%d')
    delta = (dateEd - datebuf).days
    dayAll = (day2-day1).days
    if delta/dayAll < 0.5:
        return year_floor + 0.5
    else:
        return year_floor + 1.0

def ccWorkYear(stdatestr, eddatestr):
    b8, a8 = 0, 0
    stdate=datetime.datetime.strptime(stdatestr,'%Y-%m-%d')
    eddate=datetime.datetime.strptime(eddatestr,'%Y-%m-%d')
    date81=datetime.datetime.strptime('2007-12-31','%Y-%m-%d')
    date82=datetime.datetime.strptime('2008-1-1','%Y-%m-%d')
    if stdate <= date81:
        b8delta = date81 - stdate
        b8days = b8delta.days
        b8years = b8days / 365.0
        b8 = math.ceil(b8years)
        #
        a8 = delteYearAfter08(date82,eddate)
    else:
        b8 = 0.0
        a8 = delteYearAfter08(stdate, eddate)
    b8 = float(b8)
    return b8, a8

if __name__ == '__main__':
    stdate = sys.argv[1]
    eddate = sys.argv[2]
    b8, a8 = ccWorkYear(stdate, eddate)
    print(stdate,eddate,b8, a8)
