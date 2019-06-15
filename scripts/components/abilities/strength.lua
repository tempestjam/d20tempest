name = "Strength"
shortName = "STR"

function getModifier()
    return (Strength() / 2) + TempSTRBonus()
end

function checkValueBeforeSet(val)
    return val < 100
end