name = "Charisma"
shortName = "CHA"

function getModifier()
    return math.floor((Charisma() / 2)) - 5 + TempCHABonus()
end

function checkValueBeforeSet(val)
    return val < 100
end