Кодовый замок на основе старинного дискового телефона.
Это был реальный наколеночный проект для интерактивного квеста,
был выполнен на Ардуино Мини.

Питание: внешний преобразователь AC230V-DC12V (скрыт от пользователя), 
внутренний преобразователь DC12V-DC3.3V;
Индикация:
лампа LA2 - наличие входящего напряжения 12 В;
лампа LA1 - индикация режима работы: постоянный свет - замок открыт, мигающий - режим программирования кода;
(световая индикация не использовались, было достаточно звуковых сигналов)
трубка LS1 - PWM генерация сигналов (телефонный капсюль обладает большим сопротивлением и в моём случае работал напрямую от выводов Arduino): 
постоянный гудок - готовность к набору, щелчки - во время набора, 
три сменяющихся тона дважды, затем короткие гудки - неправильно набран номер, один длинный гудок, затем короткие гудки - открытие замка);
Звонок L1-L2 - два электромагнита с якорем-качелью, звонит при поочерёдном включении, интервалы подобраны на практике - короткая трель при открытии замка;
Импульсный набор:
при снятии трубки срабатывает пружинный контакт SW3, МК переходит в ожидание набора номера, в трубке раздаётся непрерывный гудок;
при смещении диска срабатывает контакт SW2, можно использовать для определения окончания набора или заглушения гудка;
при отпускании диска контакт SW1 замыкается в соответствии с набранной цифрой (при начальном смещении импульсов нет);
я не использовал SW2, так как SW1 выдаёт импульсы со стабильным интервалом, паузы легко определяются,
в реальном проекте использовал только программный "антидребезг", на удивление работало без ошибок (контактам было более 40 лет);
Смена кода выполняется с помощью предустановленного мастер-кода, можно прибумать комбинацию со снятием трубки, смещённым диском, сбросом МК.

