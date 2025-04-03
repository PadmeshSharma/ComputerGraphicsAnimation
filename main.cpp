#include <graphics.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>

#define PI 3.14159265
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

void drawLineDDA(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1, dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xInc = dx/(float)steps, yInc = dy/(float)steps;
    float x = x1, y = y1;

    for (int i = 0; i <= steps; i++) {
        putpixel((int)(x+0.5), (int)(y+0.5), getcolor());
        x += xInc;
        y += yInc;
    }
}

void drawCircleBresenham(int xc, int yc, int r) {
    int x = 0, y = r, d = 3 - 2*r;
    while (y >= x) {
        for (int i = -1; i <= 1; i += 2)
            for (int j = -1; j <= 1; j += 2) {
                putpixel(xc + i*x, yc + j*y, getcolor());
                putpixel(xc + i*y, yc + j*x, getcolor());
            }
        x++;
        d = d > 0 ? d + 4*(x - y--) + 10 : d + 4*x + 6;
    }
}

void drawStudentBody(int x, int y, int scale, int pose) {
    // Draw different poses: 0=standing, 1=walking, 2=jumping
    if (pose == 0) { //standing
        drawLineDDA(x, y - 18*scale, x, y + 15*scale);
        drawLineDDA(x, y - 5*scale, x - 15*scale, y);
        drawLineDDA(x, y - 5*scale, x + 15*scale, y);
        drawLineDDA(x, y + 15*scale, x - 12*scale, y + 30*scale);
        drawLineDDA(x, y + 15*scale, x + 12*scale, y + 30*scale);
    } else if (pose == 1) { //walking
        drawLineDDA(x, y - 18*scale, x + 5*scale, y + 15*scale);
        drawLineDDA(x + 5*scale, y + 5*scale, x - 10*scale, y + 10*scale);
        drawLineDDA(x + 5*scale, y + 5*scale, x + 20*scale, y);
        drawLineDDA(x + 5*scale, y + 15*scale, x - 5*scale, y + 30*scale);
        drawLineDDA(x + 5*scale, y + 15*scale, x + 15*scale, y + 30*scale);
    } else {
        drawLineDDA(x, y - 18*scale, x, y + 10*scale);
        drawLineDDA(x, y - 10*scale, x - 20*scale, y - 20*scale);
        drawLineDDA(x, y - 10*scale, x + 20*scale, y - 20*scale);
        drawLineDDA(x, y + 10*scale, x - 20*scale, y + 20*scale);
        drawLineDDA(x, y + 10*scale, x + 20*scale, y + 20*scale);
    }
}

void drawStudent(int x, int y, int scale, int pose) {
    drawCircleBresenham(x, y - 30*scale, 12*scale);
    drawStudentBody(x, y, scale, pose);
}

void drawProfessor(int x, int y, int scale) {
    drawCircleBresenham(x, y - 30*scale, 15*scale);
    drawLineDDA(x, y - 15*scale, x, y + 20*scale);
    drawLineDDA(x, y - 5*scale, x - 20*scale, y + 5*scale);
    drawLineDDA(x, y - 5*scale, x + 20*scale, y + 5*scale);
    drawLineDDA(x, y + 20*scale, x - 15*scale, y + 40*scale);
    drawLineDDA(x, y + 20*scale, x + 15*scale, y + 40*scale);

    rectangle(x - 13*scale, y - 33*scale, x - 3*scale, y - 27*scale);
    rectangle(x + 3*scale, y - 33*scale, x + 13*scale, y - 27*scale);
    drawLineDDA(x - 3*scale, y - 30*scale, x + 3*scale, y - 30*scale);
}

void drawClock(int x, int y, int angle) {
    setcolor(WHITE);
    drawCircleBresenham(x, y, 30);
    drawLineDDA(x, y, x + 15*cos(angle*PI/180), y - 15*sin(angle*PI/180));
}

void drawBlackboard(int left, int top, int right, int bottom) {
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    bar(left, top, right, bottom);
    setcolor(WHITE);
    rectangle(left - 5, top - 5, right + 5, bottom + 5);
}

void drawGate() {
    int centerX = getmaxx()/2, gateWidth = 300, gateHeight = 400;
    int pillarWidth = 40, archHeight = 100;

    setcolor(BROWN);
    setfillstyle(SOLID_FILL, BROWN);
    bar(centerX - gateWidth/2, 200, centerX - gateWidth/2 + pillarWidth, 200 + gateHeight);
    bar(centerX + gateWidth/2 - pillarWidth, 200, centerX + gateWidth/2, 200 + gateHeight);

    int archPoints[10] = {
        centerX - gateWidth/2, 200,
        centerX - gateWidth/2, 200 - archHeight,
        centerX, 200 - archHeight - 50,
        centerX + gateWidth/2, 200 - archHeight,
        centerX + gateWidth/2, 200
    };
    fillpoly(5, archPoints);

    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    bar(centerX - gateWidth/2, 190, centerX + gateWidth/2, 200);

    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    setcolor(WHITE);
    outtextxy(centerX - textwidth("Banasthali Vidyapith")/2, 200 - archHeight, "Banasthali Vidyapith");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

void drawCurtains(int left, int right, int openness) {
    int centerX = (left + right)/2, curtainWidth = (right - left)/2;
    int visibleWidth = (curtainWidth * openness)/100, yPos;

    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    bar(left, 50, left + curtainWidth - visibleWidth, getmaxy()-50);
    bar(centerX + visibleWidth, 50, right, getmaxy()-50);

    setcolor(WHITE);
    drawLineDDA(left, 50, right, 50);
}

void displayText(int x, int y, char *text) {
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(x, y, text);
}

void displayTextWithDelay(int x, int y, char *text, int delayTime) {
    displayText(x, y, text);
    delay(delayTime);
}

void displayTextWithChalk(int x, int y, char *text, int delayTime) {
    cleardevice();
    drawBlackboard(100, 70, getmaxx()-100, 170);
    displayText(x, y, text);
    delay(delayTime);
}

void fireworksScene() {
    cleardevice();

    for (int i = 0; i < 5; i++) {
        cleardevice();

        setcolor(WHITE);
        for (int j = 0; j < 5; j++)
            drawStudent(150 + j*100, 250 + (i+j)%3*20 - 10, 1, (i+j)%3);

        int x = rand()%(getmaxx() - 200) + 100;
        int y = 50 + rand()%150;
        int color = rand()%15 + 1;

        setcolor(color);
        
        int baseRadius = 2;
        drawCircleBresenham(x, y, baseRadius);
        
        for (float scale = 1.0; scale <= 10.0; scale += 0.5) {
            cleardevice();  
            
            setcolor(WHITE);
            for (int j = 0; j < 5; j++)
                drawStudent(150 + j*100, 250 + (i+j)%3*20 - 10, 1, (i+j)%3);
            
            setcolor(color);
            drawCircleBresenham(x, y, (int)(baseRadius * scale));
            delay(30);
        }

        if (i%3 == 0) {
            cleardevice();
            setcolor(WHITE);
            for (int j = 0; j < 5; j++) drawStudent(150 + j*100, 250, 1, 2);
            delay(150);
        }
    }

    cleardevice();
    drawBlackboard(100, 70, getmaxx()-100, 170);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(120, 100, "Fests");
}

void studyScene() {
    for (int i = 0; i < 20; i++) {
        cleardevice();
        setcolor(WHITE);
        rectangle(50, 50, getmaxx()-50, getmaxy()-50);
        drawBlackboard(100, 70, getmaxx()-100, 170);
        displayText(220, 100, "Exams");

        rectangle(250, 280, 350, 300);

        drawCircleBresenham(320, 240, 18);
        drawLineDDA(320, 258, 320, 300);
        drawLineDDA(320, 270, 350, 250);
        drawLineDDA(320, 270, 290, 280);
        drawLineDDA(320, 300, 305, 330);
        drawLineDDA(320, 300, 335, 330);
        rectangle(350, 240, 380, 260);

        setcolor(BLACK);
        delay(100);
    }
}

void friendsScene() {
    int cx = getmaxx() / 2;
    int cy = 250;
    
    for (int i = 0; i < 30; i++) {
        cleardevice();
        
        if (i < 15) {
            // Linear approach phase
            drawStudent(100 + i*5, 250, 1, i%2);
            drawStudent(getmaxx()-100 - i*5, 250, 1, (i+1)%2);
            
            if (i > 7) {
                drawStudent(150 + (i-7)*4, 300 - (i-7)*2, 1, i%2);
                drawStudent(getmaxx()-150 - (i-7)*4, 300 - (i-7)*2, 1, (i+1)%2);
            }
        } else {
            // Circular formation phase
            int h = i - 15;
            float r = max(25, 50 - h);  
            
            if (h < 10) {
                // Moving into circle formation
                float angle = 2*PI*h/15;
                drawStudent(cx + r*sin(angle), cy - r*cos(angle), 1, h%3);
                drawStudent(cx + r*cos(angle), cy + r*sin(angle), 1, (h+1)%3);
                drawStudent(cx - r*sin(angle), cy + r*cos(angle), 1, h%3);
                drawStudent(cx - r*cos(angle), cy - r*sin(angle), 1, (h+1)%3);
            } else {
                // Final static positions
                drawStudent(cx, cy-25, 1, 0);
                drawStudent(cx+25, cy, 1, 1);
                drawStudent(cx, cy+25, 1, 2);
                drawStudent(cx-25, cy, 1, 0);
            }
        }
        delay(100);
    }
}

void professorEnter() {
    int i;
    for ( i = 0; i < 20; i++) {
        cleardevice();
        drawBlackboard(100, 70, getmaxx()-100, 170);
        displayTextWithChalk(220, 100, "First Day", 0);

        drawProfessor(150 + i*10, 200, 1);

        if (i > 5) {
            for (int s = 0; s < 3; s++) {
                drawStudent(getmaxx() - 100 - (i-6)*10 + s*40,
                            250 + (s%2)*10, 1, 0);
            }
        }
        delay(100);
    }

    for ( i = 0; i < 10; i++) {
        cleardevice();
        drawBlackboard(100, 70, getmaxx()-100, 170);
        displayTextWithChalk(220, 100, "First Day", 0);

        drawProfessor(350, 200, 1);

        for (int s = 0; s < 5; s++) {
            drawStudent(150 + s*100, 300, 1, 0);
        }
        delay(100);
    }
}

void studentsEnter() {
    for (int step = 0; step < 20; step++) {
        cleardevice();
        drawGate();

        int bx = 50 + step*15, by = getmaxy() - 100 - step*5;
        drawStudent(bx, by, 1, step%2);
        if (step > 3) drawStudent(bx - 30, by + 20, 1, (step+1)%2);
        if (step > 6) drawStudent(bx - 60, by + 40, 1, step%2);
        if (bx > getmaxx()/2 - 100) break;
        delay(150);
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TC\\BGI");

    // Scene 1: Students Arrive (Campus Gate)
    drawGate();
    studentsEnter();
    delay(1000);

    // Scene 2: First Day
    cleardevice();
    setcolor(WHITE);
    rectangle(50, 50, getmaxx()-50, getmaxy()-50);
    drawBlackboard(100, 70, getmaxx()-100, 170);

    displayTextWithChalk(220, 100, "First Day", 1000);
    delay(500);
    professorEnter();
    delay(1000);

    // Scene 3: Exams
    cleardevice();
    setcolor(WHITE);
    rectangle(50, 50, getmaxx()-50, getmaxy()-50);
    drawBlackboard(100, 70, getmaxx()-100, 170);
    drawClock(getmaxx() - 100, 100, 0);

    displayTextWithChalk(220, 100, "Exams", 1000);
    studyScene();

    // Scene 4: Fests
    displayTextWithChalk(220, 100, "Fests", 1000);
    fireworksScene();

    // Scene 5: Friends
    displayTextWithChalk(220, 100, "Friends", 1000);
    friendsScene();
    delay(300);

    // Final farewell
    char farewell[] = "Class dismissed forever.";
    int tw = textwidth(farewell), th = textheight(farewell);

    for (int i = 0; i < 10; i++) {
        setcolor(i%2 ? WHITE : YELLOW);
        outtextxy(getmaxx()/2 - tw/2, getmaxy()/2 - th/2, farewell);
        delay(200);
    }

    delay(1000);
    for (int o = 100; o >= 0; o -= 5) {
        drawCurtains(50, getmaxx()-50, o);
        delay(100);
    }

    displayTextWithDelay(getmaxx()/2 - 50, getmaxy() - 10, "Batch of 2025: Signing off", 1000);

    // Thank you message
    cleardevice();
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    char* ty1 = "Thank you for everything,";
    char* ty2 = "Banasthali. You'll always be home.";
    outtextxy(getmaxx()/2 - textwidth(ty1)/2, getmaxy()/2 - 20, ty1);
    outtextxy(getmaxx()/2 - textwidth(ty2)/2, getmaxy()/2 + 20, ty2);

    delay(3000);
    closegraph();
    return 0;
}