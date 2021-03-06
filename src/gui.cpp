#include "gui.hpp"

#include "libtcod.hpp"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include "constants.hpp"

using namespace std;

Gui::Gui() : lifeInfo(0), maxLifeInfo(0), strengthInfo(0), dexterityInfo(0), defenseInfo(0), lightOnInfo(false),
    totalRoundInfo(0.0f), speedPlayerInfo(0.0f), weaponNameInfo("Aucune")
{
    console_ = new TCODConsole(INFOS_WIDTH, INFOS_HEIGHT);
}

Gui::~Gui()
{
    delete console_;
    messages_.clearAndDelete();
}

//
// Rendu dans la scène
//  - Dessin des infos joueur
//  - Dessin des messages
//
void Gui::render()
{
    console_->setDefaultBackground(TCODColor::black);
    console_->clear();

    console_->setDefaultForeground(TCODColor::white);
    console_->print(0,  1, "Vie : %i/%i", lifeInfo, maxLifeInfo);
    console_->print(0,  3, "Mutations : x");
    console_->print(0,  4, "Vitesse   : %i", speedPlayerInfo);
    console_->print(0,  5, "Etage     : x");
    console_->print(16, 3, "For. : %i", strengthInfo);
    console_->print(16, 4, "Dex. : %i", dexterityInfo);
    console_->print(16, 5, "Def. : %i", defenseInfo);
    console_->print(0,  7, "Arme : %s", weaponNameInfo.c_str());
    console_->print(0,  9, "Temps : %.1f", totalRoundInfo);

    bar(16, 1, 10, maxLifeInfo, lifeInfo);

    //messages
    int line(1);
    float colorCoef(0.4f);
    int resHeight(0);

    for (GuiMessage** it = messages_.begin(); it != messages_.end(); it++)
    {
        GuiMessage* mess = *it;
        console_->setDefaultForeground(mess->color * colorCoef);

        resHeight = console_->printRect(MESSAGES_POS_X, line, INFOS_WIDTH - MESSAGES_POS_X - 2, 2, mess->text.c_str());

        line += resHeight;

        if (colorCoef < 1.0f)
            colorCoef += 0.08f;
    }

    //affichage de la console du GUI sur la console principale
    TCODConsole::blit(console_, 0, 0, 0, 0, TCODConsole::root, INFOS_POS_X, INFOS_POS_Y);
}

//
// Affichage d'une barre de progression sous cette forme :
//                        |#####----|
void Gui::bar(const int& px, const int& py, const int& pwidth, const float& pmax, const float& pvalue)
{
    string finalBar(pwidth, '-');
    finalBar.insert(0, "|");
    finalBar.append("|");

    int valueBar = (pvalue * pwidth) / pmax;

    for (int i = 1; i <= valueBar; i++)
        finalBar.at(i) = '#';

    console_->setDefaultForeground(TCODColor::white);
    console_->printEx(px, py, TCOD_BKGND_NONE, TCOD_LEFT, "%s", finalBar.c_str());
}

//
// Ajoute un message a la liste
// color Couleur du texte
// text Texte a afficher
//
void Gui::message(const TCODColor& pcolor, const std::string ptext, ...)
{
    //construction du texte avec les param�tres
    va_list ap;
    char buff[128];
    va_start(ap, ptext);
    vsprintf(buff, ptext.c_str(), ap);
    va_end(ap);

    //la liste est compl�te, suppression du premier
    if (messages_.size() == MESSAGES_LINES)
    {
        GuiMessage* mess = messages_.get(0);
        messages_.remove(mess);
        delete mess;
    }

    messages_.push(new GuiMessage(buff, pcolor));
}

//
// Vide la liste des messages
//
void Gui::clear()
{
    messages_.clearAndDelete();
}
