			AREA   |.text|, CODE, READONLY, ALIGN=2
			THUMB
			EXPORT Delay

Delay		PUSH {R8,R4}
			MOV  R8, #50000
wait1 		SUBS R8, #1
			BNE  wait1
			MOV  R8, #50000
wait2  		SUBS R8, #1
			BNE  wait2
			MOV  R8, #50000
wait3  		SUBS R8, #1
			BNE  wait3
			MOV  R8, #50000
wait4  		SUBS R8, #1
			BNE  wait4
			MOV  R8, #50000
wait5  		SUBS R8, #1
			BNE  wait5
			MOV  R8, #50000
wait6  		SUBS R8, #1
			BNE  wait6
			MOV  R8, #50000
wait7  		SUBS R8, #1
			BNE  wait7
			MOV  R8, #50000
wait8  		SUBS R8, #1
			BNE  wait8
			POP  {R8,R4}
			BX   LR 

			ALIGN      ; make sure the end of this section is aligned
			END        ; end of file