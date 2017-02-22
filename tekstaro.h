

const char* get_damage_mes_fmt;
// = {
//        "%s doloris je %d pointoj!\n",
//        "%s получил(а)(о) %d урона!\n",
//        "%s Suffered %d Terrible Damage!\n"};

const char* heal_damage_mes_fmt;// = {
//        "%s kuracigxis je %d pointoj!\n",
//       "%s излечился(ась)(ось) на %d урона!\n",
//        "%s %d Health Points Restored, yau!\n"};

const char* class_desc_mes[5];

void loadText(int LANG)
{
  if (LANG==0)
  {
  } //ESPERANTO
  if (LANG==1)  //ENGLISH
  {
    get_damage_mes_fmt = "%s Suffered %d Terrible Damage!\n";
    heal_damage_mes_fmt = "%s Restored %d Health Points!\n";
    class_desc_mes[0] = "Very Angry";
    class_desc_mes[1] = "Very Cunning";
    class_desc_mes[2] = "Very Faithful";
    class_desc_mes[3] = "Very Evil";
    class_desc_mes[4] = "Very Chaotic";
  } 
}

