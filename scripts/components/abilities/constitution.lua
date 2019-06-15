name = "Constitution"
shortName = "CON"

function getModifier()
    return math.floor((Constitution() / 2)) - 5 + TempCONBonus()
end

function checkValueBeforeSet(val)
    return val < 100
end