#ifndef KSTYLE_H
#define KSTYLE_H

//custom headers
#include "KMap.h"

//ROOT headers
#include <TROOT.h>
#include <TH1.h>

//STL headers
#include <string>

class KStyle {
	public:
		//constructor
		KStyle() : name(""), defaultOpt(new OptionMap()), localOpt(new OptionMap()) {}
		KStyle(string name_, OptionMap* defaultOpt_, OptionMap* localOpt_) : 
			name(name_), defaultOpt(defaultOpt_ ? defaultOpt_ : new OptionMap()), localOpt(localOpt_ ? localOpt_ : new OptionMap()),
			linecolor(kBlack), markercolor(kBlack), fillcolor(kWhite), errcolor(kGray+1),
			linewidth(2), linestyle(1), markersize(1.), markerstyle(20), fillstyle(1001), errstyle(3005),
			drawopt("pe"), legopt("pe"), errdrawopt("f"), errlegopt("f")
		{
			//initialize all params: localOpt overrides defaultOpt
			CheckAllParams(true);
		}
		//copy constructor
		KStyle(const KStyle& orig) : 
			name(orig.name), defaultOpt(orig.defaultOpt), localOpt(orig.localOpt), linecolor(orig.linecolor), markercolor(orig.markercolor), fillcolor(orig.fillcolor), errcolor(orig.errcolor),
			linewidth(orig.linewidth), linestyle(orig.linestyle), markersize(orig.markersize), markerstyle(orig.markerstyle), fillstyle(orig.fillstyle), errstyle(orig.errstyle),
			drawopt(orig.drawopt), legopt(orig.legopt), errdrawopt(orig.errdrawopt), errlegopt(orig.errlegopt)
			{}
		//destructor
		virtual ~KStyle() {}
		
		//initialization
		void CheckAllParams(bool checkdefault);
		
		//accessors
		string GetDrawOpt(string suff="") { return drawopt + (suff.size()>0 ? " "+suff : ""); }
		string GetLegOpt() { return legopt; }
		string GetDrawOptErr(string suff="") { return errdrawopt + (suff.size()>0 ? " "+suff : ""); }
		string GetLegOptErr() { return errlegopt; }
		template <class T>
		void Format(T* obj){
			//set the style of a hist or graph
			obj->SetLineColor(linecolor);
			obj->SetLineWidth(linewidth);
			obj->SetLineStyle(linestyle);
			obj->SetMarkerColor(markercolor);
			obj->SetMarkerSize(markersize);
			obj->SetMarkerStyle(markerstyle);
			obj->SetFillColor(fillcolor);
			obj->SetFillStyle(fillstyle);
		}
		template <class T>
		void FormatErr(T* obj){
			//set the style of an error band
			obj->SetFillColor(errcolor);
			obj->SetFillStyle(errstyle);
		}
		void SetLocalOpt(OptionMap* localOpt_){
			localOpt = localOpt_ ? localOpt_ : new OptionMap();
			//check for changed style
			if(localOpt_) CheckAllParams(false);
		}
		
	private:
		//helper functions
		template <class T>
		void CheckParam(string sname, T & xtype, bool checkdefault){
			if(checkdefault) defaultOpt->Get(sname,xtype);
			localOpt->Get(sname,xtype);
		}
	
		//members
		string name;
		OptionMap *defaultOpt, *localOpt;
		//style params
		Color_t linecolor, markercolor, fillcolor, errcolor;
		int linewidth, linestyle;
		float markersize;
		int markerstyle;
		int fillstyle, errstyle;
		string drawopt, legopt, errdrawopt, errlegopt;
};

//specialization for colors
template <>
void KStyle::CheckParam<Color_t>(string sname, Color_t & xtype, bool checkdefault){
	//first check "color", then specific ___color
	if(checkdefault){
		defaultOpt->Get("color",xtype);
		defaultOpt->Get(sname,xtype);
	}
	localOpt->Get("color",xtype);
	localOpt->Get(sname,xtype);
}

void KStyle::CheckAllParams(bool checkdefault){
	//colors
	CheckParam("linecolor",linecolor,checkdefault);
	CheckParam("markercolor",markercolor,checkdefault);
	CheckParam("fillcolor",fillcolor,checkdefault);
	CheckParam("errcolor",errcolor,checkdefault);
	
	//lines
	CheckParam("linewidth",linewidth,checkdefault);
	CheckParam("linestyle",linestyle,checkdefault);
	
	//markers
	CheckParam("markersize",markersize,checkdefault);
	CheckParam("markerstyle",markerstyle,checkdefault);
	
	//fills
	CheckParam("fillstyle",fillstyle,checkdefault);
	CheckParam("errstyle",errstyle,checkdefault);
	
	//opts
	CheckParam("drawopt",drawopt,checkdefault);
	CheckParam("legopt",legopt,checkdefault);
	CheckParam("errdrawopt",errdrawopt,checkdefault);
	CheckParam("errlegopt",errlegopt,checkdefault);
}

#endif