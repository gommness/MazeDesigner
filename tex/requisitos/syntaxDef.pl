startCondition --> condition.
startCondition --> [].
condition --> simpleCondition.
condition --> compositeCondition.
simpleCondition --> have, number, item.
have --> [have].
have --> [].
number --> numberConstant.
number --> [].
numberConstant --> nonZeroDigit.
numberConstant --> nonZeroDigit, digits.
nonZeroDigit --> [1].
nonZeroDigit --> [2].
nonZeroDigit --> [3].
nonZeroDigit --> [4].
nonZeroDigit --> [5].
nonZeroDigit --> [6].
nonZeroDigit --> [7].
nonZeroDigit --> [8].
nonZeroDigit --> [9].
digits --> digit.
digits --> digit, digits.
digit --> nonZeroDigit.
digit --> [0].
item --> [key].
compositeCondition --> conditionList.
compositeCondition --> conditionList, connector, compositeCondition.
compositeCondition --> ["("], conditionList, [")"], connector, compositeCondition.
compositeCondition --> ["("], compositeCondition, [")"].
conditionList --> simpleCondition.
conditionList --> simpleCondition, connector, conditionList.
connector --> [or].
connector --> [and].