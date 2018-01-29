핵심 API로 배우는 윈도우 프로그래밍(강경우, 한빛아카데미)

------

**1. 윈도우 프로그래밍기초■ 윈도우 SDK : 윈도우 API로 구현한 라이브러리나 헤더파일들■ 윈도우 프로그램의 특징GUI기반 프로그램이벤트 기반 프로그램 : 윈도우가 이벤트를 감지하고 메시지를 보내면 윈도우 프로그램은 수신한 메시지에 따라 반응함멀티쓰레드 기반 프로그램■ 윈도우 프로그램에서 메시지 처리과정![img](https://lh5.googleusercontent.com/fV0PPDoege5JHWydot8HXoXaVF1prRpCttkWdz2Nh03otSY-4DtQH1PW0NejER4rhtQe9rXHYEu2xTevz5fY4fpEhYo3XqZVLEPfG7pPQkDEuBSbtYx0GBmB-Qh6JGM7K_-Nsn_h)3. 정수값 메시지가 큐에 쌓임4. WinMain() 함수는 큐에서 메시지를 차례로 꺼냄5. 꺼낸 메시지를 해석해 메시지 처리함수로 보냄. GetMessage() 함수로 메시지를 꺼낸 후 메시지는 msg에 저장한다. TranslateMessage()함수로 변형하여 DispatchMessage()함수로 메시지를 WinProc()로 보냄.6. 메시지를 받아서 반응을 보임■ 윈도우 프로그램의 기본구조- 메인부분에서는 윈도우 클래스를 생성 및 등록, 등록된 윈도우 클래스를 기반으로 윈도우를 생성- 윈도우에서 발생한 메시지는 메시지 큐에 저장되고 메시지 처리 부분이 하나씩 처리- 메시지 처리부분은 처리할 메시지에 대해서만 작성하고, 나머지 메시지 처리는 윈도우 커널에서 일임**