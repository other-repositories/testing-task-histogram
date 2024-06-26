## О приложении
Приложение предназначено для визуализации самых часто встречаемых слов в тексте. 

## Зависимости
`Qt6.2+`
`C++17`

## Установка
- Установить необходимые зависимости и собрать `CMake`

или
- Скачать release

## Архитектура
![image](https://github.com/other-repositories/testing-task-histogram/assets/92841151/786a6242-faa3-4d95-bf08-36ab92ab76e0)

## Логика работы
- Загрузите исходный файл любого формата, где содержится какой-либо текст
- Нажмитие старт для анализа текста
- Вы можете поставить на паузу, либо оставить анализ во время исполнения
- При нажатии кнопки отмена - процесс прекратиться, и будет возможно посмотерть результаты на момент остановки
- Также есть возможность остановки, если вы находитесь в состоянии паузы
- Для повторного анализа выберите другой файл, либо можете повторить текущий после полного завершения процесса
