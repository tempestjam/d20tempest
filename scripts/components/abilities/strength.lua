name = "Strength"
shortName = "STR"

function getModifier()
    return math.floor((Strength() / 2)) - 5 + TempSTRBonus()
end

function checkValueBeforeSet(val)
    return true
end