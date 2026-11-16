#include "common.h"
#include "gph.h"
#include "field/n20.h"

void EvtCharSelPlace(s8 idx, u32 mode) {
    switch (mode) {
    case 0:
        evtCharSelWork->chars[idx]->moveDir = 0xCCCC;
        evtCharSelWork->chars[idx]->model->pos[0] = -162.0f;
        evtCharSelWork->chars[idx]->model->pos[1] = 0.0f;
        evtCharSelWork->chars[idx]->model->pos[2] = -53.0f;
        evtCharSelWork->chars[idx]->model->pos[3] = 0.0f;
        break;
    case 1:
        evtCharSelWork->chars[idx]->moveDir = 0xB333;
        evtCharSelWork->chars[idx]->model->pos[0] = -162.0f;
        evtCharSelWork->chars[idx]->model->pos[1] = 0.0f;
        evtCharSelWork->chars[idx]->model->pos[2] = 53.0f;
        evtCharSelWork->chars[idx]->model->pos[3] = 0.0f;
        break;
    case 2:
        evtCharSelWork->chars[idx]->moveDir = 0x9999;
        evtCharSelWork->chars[idx]->model->pos[0] = -100.0f;
        evtCharSelWork->chars[idx]->model->pos[1] = 0.0f;
        evtCharSelWork->chars[idx]->model->pos[2] = 138.0f;
        evtCharSelWork->chars[idx]->model->pos[3] = 0.0f;
        break;
    case 3:
        evtCharSelWork->chars[idx]->moveDir = 0x8000;
        evtCharSelWork->chars[idx]->model->pos[0] = 0.0f;
        evtCharSelWork->chars[idx]->model->pos[1] = 0.0f;
        evtCharSelWork->chars[idx]->model->pos[2] = 170.0f;
        evtCharSelWork->chars[idx]->model->pos[3] = 0.0f;
        break;
    case 4:
        evtCharSelWork->chars[idx]->moveDir = 0x6666;
        evtCharSelWork->chars[idx]->model->pos[0] = 100.0f;
        evtCharSelWork->chars[idx]->model->pos[1] = 0.0f;
        evtCharSelWork->chars[idx]->model->pos[2] = 138.0f;
        evtCharSelWork->chars[idx]->model->pos[3] = 0.0f;
        break;
    }
    evtCharSelWork->chars[idx]->heading = evtCharSelWork->chars[idx]->moveDir;
}
