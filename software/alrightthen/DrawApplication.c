

//void testTouch()
//{
//	uint32_t x,y;
//	uint16_t dotColor = GREEN;
//	//uint32_t numTouchs = 0;
//	while(1)
//	{
//
////		numTouchs = (IORD_32DIRECT(I2C_DATA_REG_0_BASE,1));
////		dotColor = color[1];
//		x = IORD_32DIRECT(I2C_DATA_REG_0_BASE,0);
//		if (x & 0x80000000)
//		{
//			y =  (x & 0x0FFF);
//			x = ((x>>16));
//
//			drawFrame(BLACK, 1);
//			drawCircleFill(dotColor, (x&0x0FFF), y, 30, 1);
//
//			usleep(10);
//			flipFrame(1);
//
//			x = IORD_32DIRECT(I2C_DATA_REG_0_BASE,0);
//			y =  (x & 0x0FFF);
//			x = ((x>>16));
//
//			drawFrame(BLACK, 0);
//			drawCircleFill(dotColor, (x&0x0FFF), y, 30, 0);
//
//			usleep(10);
//			flipFrame(0);
//
//
//			x = IORD_32DIRECT(I2C_DATA_REG_0_BASE,0);
//			y =  (x & 0x0FFF);
//			x = ((x>>16));
//
//
//			drawFrame(BLACK, 1);
//			drawCircleFill(dotColor, (x&0x0FFF), y, 30, 1);
//
//
//			usleep(10);
//			flipFrame(1);
//
//
//			x = IORD_32DIRECT(I2C_DATA_REG_0_BASE,0);
//			y =  (x & 0x0FFF);
//			x = ((x>>16));
//
//			drawFrame(BLACK, 0);
//			drawCircleFill(dotColor, (x&0x0FFF), y, 30, 0);
//			flipFrame(0);
//			usleep(10);
//			//printf("Touch x: %lX\t\tTouch y: %lX\n", x, y);
//		}
//		else
//		{
//			drawFrame(BLACK, 1);
//			flipFrame(1);
//		}
//	}
//}
//
//void DrawApplication()
//{
//	int x, y;
//	uint16_t dotColor = RED;
//	uint16_t dotSize = 18;
//
//	drawFrame(WHITE,0);
//	DrawColorPicker(0);
//	flipFrame(0);
//	drawFrame(WHITE,1);
//	DrawColorPicker(1);
//	while (1)
//	{
//		x = grabTouchData(0);
//		y = (x & 0x0FFF);
//		x = ((x>>16));
//
//		//draw
//		while (x & 0x8000){
//			if (y > 40){
////				while (x & 0x8000){
//					x = x & 0x0FFF;
////					drawCircleFill(dotColor, x, y, dotSize, 1);
////					DrawColorPicker(1);
////					flipFrame(1);
//
//					drawCircleFill(dotColor, x, y, dotSize, 0);
//					DrawColorPicker(0);
////					flipFrame(0);
//
//					x = grabTouchData(0);
//					y = (x & 0x0FFF);
//					x = ((x>>16));
////				}
//			}
//			else{
//				switch ((x & 0x0FFF)/COLORPICKERSIZE){
//					case 0:
//						dotColor = COLORS[0];
//						break;
//					case 1:
//						dotColor = COLORS[1];
//						break;
//					case 2:
//						dotColor = COLORS[2];
//						break;
//					case 3:
//						dotColor = WHITE;
//						break;
//					case 4:
//						drawFrame(WHITE,0);
//						DrawColorPicker(0);
//						break;
//					default:
//						dotColor = COLORS[4];
//				}
//				/*	Touch Lockout   */
//				while (x & 0x80000000)
//					x = grabTouchData(0);
//			}
//		}
//	}
//}
//
//void DrawColorPicker(uint8_t frame)
//{
//	drawRectangleFill(COLORS[0], 0, 0, COLORPICKERSIZE, 40, frame);
//	drawRectangleFill(COLORS[1], COLORPICKERSIZE, 0, (COLORPICKERSIZE)*2, 40, frame);
//	drawRectangleFill(COLORS[2], COLORPICKERSIZE*2, 0, (COLORPICKERSIZE)*3, 40, frame);
//	drawRectangleFill(WHITE, COLORPICKERSIZE*3, 0, (COLORPICKERSIZE)*4, 40, frame);
//	drawRectangleFill(GREY, COLORPICKERSIZE*4, 0, (COLORPICKERSIZE)*5, 40, frame);
//}