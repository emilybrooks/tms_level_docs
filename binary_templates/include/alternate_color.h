local uint32 count = 0;
uint32 AlternateColor()
{
    if (count == 0)
    {
        count += 1;
        return cDkRed;
    }
    else if (count == 1)
    {
        count += 1;
        return cDkGreen;
    }
    else if (count == 2)
    {
        count += 1;
        return cDkBlue;
    }
    else if (count == 3)
    {
        count += 1;
        return cDkYellow;
    }
    else
    {
        count = 0;
        return cDkAqua;
    }
}
