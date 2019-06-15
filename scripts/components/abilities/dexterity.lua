name = "Dexterity"
shortName = "DEX"

function getModifier()
    return math.floor((Dexterity() / 2)) - 5 + TempDEXBonus()
end

function checkValueBeforeSet(val)
    return val < 100
end