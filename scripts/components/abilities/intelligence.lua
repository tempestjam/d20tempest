name = "Intelligence"
shortName = "INT"

function getModifier()
    return math.floor((Intelligence() / 2)) - 5 + TempINTBonus()
end

function checkValueBeforeSet(val)
    return val < 100
end