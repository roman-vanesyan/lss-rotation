Программа решает СЛАУ методом вращений.

===============================================================================

Алгоритм:

Пусть дана СЛАУ:
```
A_11*X_1+A_12*X_2+...A_1m*Xm=F_1
A_21*X_1+A_22*X_2+...A_2m*Xm=F_2
...
A_m1*X_1+A_m2*X_2+...A_mm*X_m=F_m
```
Требуется найти вектор решений (X_1, ..., X_m).

Прямой ход:

Положим
```
C_i = A_mm / sqrt(A_mm^2 + A_m+1m^2)
S_i = A_m+1m / sqrt(A_mm^2 + A_m+1m^2)
```

и

```
A_(k)_ij = C_k * A_(k-1)_ij + S_k * A_(k-1)_i+1j
A_(k)_i+1j = -S_k * A_(k-1)_ij + S_k * A_(k-1)_i+1j, j = 1...m, k = 1...m-1
F_(k)_i = C_k * F_(k-1)_i + S_k * F_(k-1)_i+1
F_(k)_i+1 = -S_k * F_(k-1)_i + S_k * F_(k-1)_i+1, j = 1...m, k = 1...m-1
```

где i = 1...m-1

Заменим l-е уравнение системы результатом сложения l-го уравнения с n-ым уравнением, где l=1...m-1, n=l+1...m
Выполняем преобразование l-1 для каждой l-ой строки. Проделав m-1 подобных операций получим верхнетреугольную
матрицу матрицу.

Обратный ход:
```
X_m = F_(m-1)_m / a_(m-1)_mm
X_1 = (F_1 - A_12*X_2 - ... - A_1m*X_m) / A_11
X_2 = (F_(1)_2 - A_23*X_3 - ... - A_2m*X_m) / A_(1)_22
...
```

===============================================================================

Коды ошибок:
* `0` - работа завершена успешно, решение построено
* `1` - работа завершена успешно, решения системы не существует
* `100` - ошибка при открытии/закрытии файла
* `101` - ошибка при чтении входных данных из файла
* `102` - ошибка при записи выходных данных в файл
* `200` - ошибка последовательности задания аргументов (флаги могут заданы только после позиционных аргументов, если они есть)
* `201` - слишком много позиционных аргументов
* `202` - был задан неизвестный флаг

===============================================================================

`int lss(int n, double *A, double *B, double *X, double *tmp)`
Решает СЛАУ методом вращений.
`n` - размер матрицы, вектора значений, вектора решений
`A` - матрица коэфициентов
`B` - вектор значений
`X` - вектор решений
`tmp` - вспомогательный массив

