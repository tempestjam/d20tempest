name = "Wisdom"
shortName = "WIS"

function getModifier()
    return math.floor((Wisdom() / 2)) - 5 + TempWISBonus()
end

function checkValueBeforeSet(val)
    return val < 100
end