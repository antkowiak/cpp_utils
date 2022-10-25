#pragma once

//
// htmldoc.h - Utility to generate html pages.
//
// Written by Ryan Antkowiak
//
// 2020-10-24
//

#include <initializer_list>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "htmlchars.h"

namespace rda
{
	namespace htmldoc
	{
		class attribute
		{
		public:
			attribute(const std::string& key, const std::string& value) : m_key(key), m_value(value) {}
			attribute(const attribute& rhs) : m_key(rhs.m_key), m_value(rhs.m_value) {}
			virtual void set_value(const std::string& value) { m_value = value; }
			virtual ~attribute() = default;
			std::string get_key() const { return m_key; }
			std::string get_value() const { return m_value; }
		protected:
			std::string m_key;
			std::string m_value;
		}; // class attribute

		std::string replace_all_substr(std::string& str, const std::string& from, const std::string& to)
		{
			std::string retStr(str);

			if (from.empty())
				return retStr;

			size_t pos = 0;
			while ((pos = retStr.find(from, pos)) != std::string::npos)
			{
				retStr.replace(pos, from.length(), to);
				pos += to.length();
			}
			return retStr;
		}

		std::string prepare_content(const std::string& content, bool convertSpaces = true)
		{
			std::string output(content);
			output = replace_all_substr(output, "&", rda::htmlchars::SYM_AMPERSAND);
			output = replace_all_substr(output, "<", rda::htmlchars::SYM_LESS_THAN_SIGN);
			output = replace_all_substr(output, ">", rda::htmlchars::SYM_GREATER_THAN_SIGN);
			output = replace_all_substr(output, "\"", rda::htmlchars::SYM_AMPERSAND);
			if (convertSpaces)
				output = replace_all_substr(output, " ", rda::htmlchars::SYM_NO_BREAK_SPACE);
			return output;
		}

		class node
		{
		public:
			node() = delete;
			node(const std::string& name) : m_name(name) {}
			node(const std::string& name, const std::string& content) : m_name(name), m_content(content) {}
			virtual ~node() = default;
			void set_content(const std::string& content) { m_children.clear(); m_content = content; }
			void set_content(const int content) { m_children.clear(); m_content = std::to_string(content); }
			void set_content(const double content) { m_children.clear(); m_content = std::to_string(content); }
			virtual void add_attribute(const attribute& attrib) { m_attributes.push_back(std::make_shared<attribute>(attrib)); }
			virtual void add_attribute(const std::string& key, const std::string& value) { m_attributes.push_back(std::make_shared<attribute>(key, value)); }
			virtual void add_attribute(const std::string& key, const int value) { m_attributes.push_back(std::make_shared<attribute>(key, std::to_string(value))); }
			virtual void add_attribute(const std::string& key, const double value) { m_attributes.push_back(std::make_shared<attribute>(key, std::to_string(value))); }
			virtual void add_attribute(std::shared_ptr<attribute> pAttribute) { m_attributes.push_back(pAttribute); }
			virtual void add_style(const std::string& style) { m_styles.push_back(style); }
			virtual void add_comment(const std::string& comment) { m_comments.push_back(comment); }
			virtual void add_child(std::shared_ptr<node> pNode) { m_content.clear(); if (pNode) m_children.push_back(pNode); }
			virtual void add_children(std::vector<std::shared_ptr<node>>& children) { for (auto c : children) add_child(c); }

		public:

			virtual std::string to_string() const
			{
				std::stringstream ss;
				ss << "<" << m_name;
				if (!m_styles.empty())
				{
					ss << " class='";
					bool first = true;

					for (const auto& style : m_styles)
					{
						if (!first)
							ss << " ";
						first = false;
						ss << style;
					}
					ss << "'";
				}

				for (const auto& attribute : m_attributes)
					ss << " " << attribute->get_key() << "='" << attribute->get_value() << "'";

				if (!m_content.empty())
				{
					ss << ">";
					for (const auto& comment : m_comments)
						ss << "<!--" << comment << "-->";
					ss << m_content;
					ss << "</" << m_name << ">";
				}
				else if (!m_children.empty())
				{
					ss << ">";
					for (const auto& comment : m_comments)
						ss << "<!--" << comment << "-->";
					for (auto& child : m_children)
						if (child)
							ss << child->to_string();
					ss << "</" << m_name << ">";
				}
				else
				{
					ss << " />";
					for (const auto& comment : m_comments)
						ss << "<!--" << comment << "-->";
				}

				return ss.str();
			}

			virtual std::string to_pretty_string(const size_t indent = 0) const
			{
				std::string indentStr = "";
				for (size_t i = 0; i < indent; ++i)
					indentStr += "  ";
				std::string indentStrContent = indentStr + "  ";
				std::stringstream ss;
				ss << indentStr << "<" << m_name;

				if (!m_styles.empty())
				{
					ss << " class='";
					bool first = true;

					for (const auto& style : m_styles)
					{
						if (!first)
							ss << " ";
						first = false;
						ss << style;
					}
					ss << "'";
				}

				for (const auto& attribute : m_attributes)
					ss << " " << attribute->get_key() << "='" << attribute->get_value() << "'";

				if (!m_content.empty())
				{
					ss << ">" << std::endl;
					for (const auto& comment : m_comments)
						ss << indentStrContent << "<!--" << comment << "-->" << std::endl;
					ss << indentStrContent << m_content << std::endl;
					ss << indentStr << "</" << m_name << ">";
				}
				else if (!m_children.empty())
				{
					ss << ">" << std::endl;
					for (const auto& comment : m_comments)
						ss << indentStrContent << "<!--" << comment << "-->" << std::endl;
					for (auto& child : m_children)
						if (child)
							ss << child->to_pretty_string(indent + 1);
					ss << indentStr << "</" << m_name << ">";
				}
				else
				{
					ss << " />";
					for (const auto& comment : m_comments)
						ss << std::endl << indentStrContent << "<!--" << comment << "-->";

				}

				ss << std::endl;
				return ss.str();
			}

		protected:
			std::string m_name;
			std::string m_content;
			std::vector<std::shared_ptr<attribute>> m_attributes;
			std::vector<std::string> m_styles;
			std::vector<std::string> m_comments;
			std::vector<std::shared_ptr<node>> m_children;
		}; // class node

		typedef std::shared_ptr<node> TNode;

#define NODE_FACTORY(NODE_NAME) \
        class node_##NODE_NAME : public node                                                                            \
        {                                                                                                               \
            public:                                                                                                     \
                node_##NODE_NAME() : node(#NODE_NAME) {}                                                                \
                                                                                                                        \
            static std::shared_ptr<node_##NODE_NAME> create()                                                           \
            {                                                                                                           \
                return std::make_shared<node_##NODE_NAME>();                                                            \
            }                                                                                                           \
                                                                                                                        \
            static std::shared_ptr<node_##NODE_NAME> create(std::shared_ptr<node> pParent)                              \
            {                                                                                                           \
                auto pNode = std::make_shared<node_##NODE_NAME>();                                                      \
                pParent->add_child(pNode);                                                                              \
                return pNode;                                                                                           \
            }                                                                                                           \
                                                                                                                        \
            static std::shared_ptr<node_##NODE_NAME> create(std::shared_ptr<node> pParent,                              \
                                                            const std::initializer_list<std::string> & styles)          \
            {                                                                                                           \
                auto pNode = std::make_shared<node_##NODE_NAME>();                                                      \
                for (const auto & style : styles)                                                                       \
                    pNode->add_style(style);                                                                            \
                if (pParent)                                                                                            \
                    pParent->add_child(pNode);                                                                          \
                return pNode;                                                                                           \
            }                                                                                                           \
                                                                                                                        \
            static std::shared_ptr<node_##NODE_NAME> create(std::shared_ptr<node> pParent,                              \
                                                            const std::initializer_list<std::string> & styles,          \
                                                            const std::initializer_list<attribute> & attributes)        \
            {                                                                                                           \
                auto pNode = std::make_shared<node_##NODE_NAME>();                                                      \
                for (const auto & style : styles)                                                                       \
                    pNode->add_style(style);                                                                            \
                for (const auto & attrib : attributes)                                                                  \
                    pNode->add_attribute(attrib);                                                                       \
                if (pParent)                                                                                            \
                    pParent->add_child(pNode);                                                                          \
                return pNode;                                                                                           \
            }                                                                                                           \
                                                                                                                        \
            static std::shared_ptr<node_##NODE_NAME> create(std::shared_ptr<node> pParent,                              \
                                                            const std::initializer_list<std::string> & styles,          \
                                                            const std::initializer_list<attribute> & attributes,        \
                                                            const std::string & content)                                \
            {                                                                                                           \
                auto pNode = std::make_shared<node_##NODE_NAME>();                                                      \
                for (const auto & style : styles)                                                                       \
                    pNode->add_style(style);                                                                            \
                for (const auto & attrib : attributes)                                                                  \
                    pNode->add_attribute(attrib);                                                                       \
                pNode->set_content(content);                                                                            \
                if (pParent)                                                                                            \
                    pParent->add_child(pNode);                                                                          \
                return pNode;                                                                                           \
            }                                                                                                           \
                                                                                                                        \
        };

	NODE_FACTORY(a)
	NODE_FACTORY(abbr)
	NODE_FACTORY(acronym)
	NODE_FACTORY(address)
	NODE_FACTORY(applet)
	NODE_FACTORY(area)
	NODE_FACTORY(article)
	NODE_FACTORY(aside)
	NODE_FACTORY(audio)
	NODE_FACTORY(b)
	NODE_FACTORY(base)
	NODE_FACTORY(basefont)
	NODE_FACTORY(bdi)
	NODE_FACTORY(bdo)
	NODE_FACTORY(big)
	NODE_FACTORY(blockquote)
	NODE_FACTORY(body)
	NODE_FACTORY(br)
	NODE_FACTORY(button)
	NODE_FACTORY(canvas)
	NODE_FACTORY(caption)
	NODE_FACTORY(center)
	NODE_FACTORY(cite)
	NODE_FACTORY(code)
	NODE_FACTORY(col)
	NODE_FACTORY(colgroup)
	NODE_FACTORY(data)
	NODE_FACTORY(datalist)
	NODE_FACTORY(dd)
	NODE_FACTORY(del)
	NODE_FACTORY(details)
	NODE_FACTORY(dfn)
	NODE_FACTORY(dialog)
	NODE_FACTORY(dir)
	NODE_FACTORY(div)
	NODE_FACTORY(dl)
	NODE_FACTORY(dt)
	NODE_FACTORY(em)
	NODE_FACTORY(embed)
	NODE_FACTORY(fieldset)
	NODE_FACTORY(figcaption)
	NODE_FACTORY(figure)
	NODE_FACTORY(font)
	NODE_FACTORY(footer)
	NODE_FACTORY(form)
	NODE_FACTORY(frame)
	NODE_FACTORY(frameset)
	NODE_FACTORY(h1)
	NODE_FACTORY(h2)
	NODE_FACTORY(h3)
	NODE_FACTORY(h4)
	NODE_FACTORY(h5)
	NODE_FACTORY(h6)
	NODE_FACTORY(head)
	NODE_FACTORY(header)
	NODE_FACTORY(hr)
	NODE_FACTORY(html)
	NODE_FACTORY(i)
	NODE_FACTORY(iframe)
	NODE_FACTORY(img)
	NODE_FACTORY(input)
	NODE_FACTORY(ins)
	NODE_FACTORY(kbd)
	NODE_FACTORY(label)
	NODE_FACTORY(legend)
	NODE_FACTORY(li)
	NODE_FACTORY(link)
	NODE_FACTORY(main)
	NODE_FACTORY(map)
	NODE_FACTORY(mark)
	NODE_FACTORY(meta)
	NODE_FACTORY(meter)
	NODE_FACTORY(nav)
	NODE_FACTORY(noframes)
	NODE_FACTORY(noscript)
	NODE_FACTORY(object)
	NODE_FACTORY(ol)
	NODE_FACTORY(optgroup)
	NODE_FACTORY(option)
	NODE_FACTORY(output)
	NODE_FACTORY(p)
	NODE_FACTORY(param)
	NODE_FACTORY(picture)
	NODE_FACTORY(pre)
	NODE_FACTORY(progress)
	NODE_FACTORY(q)
	NODE_FACTORY(rp)
	NODE_FACTORY(rt)
	NODE_FACTORY(ruby)
	NODE_FACTORY(s)
	NODE_FACTORY(samp)
	NODE_FACTORY(script)
	NODE_FACTORY(section)
	NODE_FACTORY(select)
	NODE_FACTORY(small)
	NODE_FACTORY(source)
	NODE_FACTORY(span)
	NODE_FACTORY(strike)
	NODE_FACTORY(strong)
	NODE_FACTORY(style)
	NODE_FACTORY(sub)
	NODE_FACTORY(summary)
	NODE_FACTORY(sup)
	NODE_FACTORY(svg)
	NODE_FACTORY(table)
	NODE_FACTORY(tbody)
	NODE_FACTORY(td)
	NODE_FACTORY(template)
	NODE_FACTORY(textarea)
	NODE_FACTORY(tfoot)
	NODE_FACTORY(th)
	NODE_FACTORY(thead)
	NODE_FACTORY(time)
	NODE_FACTORY(title)
	NODE_FACTORY(tr)
	NODE_FACTORY(track)
	NODE_FACTORY(tt)
	NODE_FACTORY(u)
	NODE_FACTORY(ul)
	NODE_FACTORY(var)
	NODE_FACTORY(video)
	NODE_FACTORY(wbr)

		typedef std::initializer_list<std::string> TStyles;
		typedef std::initializer_list<attribute> TAttribs;
		typedef std::initializer_list<attribute> TAttributes;

		constexpr const TStyles NO_STYLES = {};
		constexpr const TAttributes NO_ATTRIBS = {};
		constexpr const TAttributes NO_ATTRIBUTES = {};

		class document
		{
		public:
			document() {}
			document(const std::string& doctype) : m_doctype(doctype) {}
			document(const std::string& doctype, std::shared_ptr<node_html> root_node) : m_doctype(doctype), m_root_node(root_node) {}
			virtual ~document() = default;
			void set_doctype(const std::string& doctype) { m_doctype = doctype; }
			void add_comment(const std::string& comment) { m_comments.push_back(comment); }
			void set_root_node(std::shared_ptr<node_html> root_node) { m_root_node = root_node; }
			void print() const { std::cout << to_string(); }
			void pretty_print() const { std::cout << to_pretty_string(); }

			std::string to_string() const
			{
				std::stringstream ss;
				ss << "<!DOCTYPE " << m_doctype << ">" << std::endl;
				for (const auto& comment : m_comments)
					ss << "<!--" << comment << "-->";
				ss << m_root_node->to_string();
				return ss.str();
			}

			std::string to_pretty_string() const
			{
				std::stringstream ss;
				ss << "<!DOCTYPE " << m_doctype << ">" << std::endl;
				for (const auto& comment : m_comments)
					ss << "<!--" << comment << "-->" << std::endl;
				ss << m_root_node->to_pretty_string();
				return ss.str();
			}

		protected:
			std::string m_doctype = "html";
			std::vector<std::string> m_comments;
			std::shared_ptr<node_html> m_root_node;
		}; // class document


		struct attribute_info
		{
			const char* const name;
			const std::initializer_list<const char* const> valid_tags;
			const char* const description;
		};

		const std::initializer_list<attribute_info> ATTRIBUTE_INFO =
		{
                    {"accesskey", {"*"},"Specifies a shortcut key to activate/focus an element"},
                    {"class", {"*"},"Specifies one or more classnames for an element (refers to a class in a style sheet)"},
                    {"contenteditable", {"*"},"Specifies whether the content of an element is editable or not"},
                    {"dir", {"*"},"Specifies the text direction for the content in an element"},
                    {"draggable", {"*"},"Specifies whether an element is draggable or not"},
                    {"hidden", {"*"},"Specifies that an element is not yet, or is no longer, relevant"},
                    {"id", {"*"},"Specifies a unique id for an element"},
                    {"lang", {"*"},"Specifies the language of the element's content"},
                    {"onblur", {"*"},"Script to be run when the element loses focus"},
                    {"onchange", {"*"},"Script to be run when the value of the element is changed"},
                    {"onclick", {"*"},"Script to be run when the element is being clicked"},
                    {"oncontextmenu", {"*"},"Script to be run when a context menu is triggered"},
                    {"oncopy", {"*"},"Script to be run when the content of the element is being copied"},
                    {"oncut", {"*"},"Script to be run when the content of the element is being cut"},
                    {"ondblclick", {"*"},"Script to be run when the element is being double-clicked"},
                    {"ondrag", {"*"},"Script to be run when the element is being dragged"},
                    {"ondragend", {"*"},"Script to be run at the end of a drag operation"},
                    {"ondragenter", {"*"},"Script to be run when an element has been dragged to a valid drop target"},
                    {"ondragleave", {"*"},"Script to be run when an element leaves a valid drop target"},
                    {"ondragover", {"*"},"Script to be run when an element is being dragged over a valid drop target"},
                    {"ondragstart", {"*"},"Script to be run at the start of a drag operation"},
                    {"ondrop", {"*"},"Script to be run when dragged element is being dropped"},
                    {"onfocus", {"*"},"Script to be run when the element gets focus"},
                    {"oninput", {"*"},"Script to be run when the element gets user input"},
                    {"oninvalid", {"*"},"Script to be run when the element is invalid"},
                    {"onkeydown", {"*"},"Script to be run when a user is pressing a key"},
                    {"onkeypress", {"*"},"Script to be run when a user presses a key"},
                    {"onkeyup", {"*"},"Script to be run when a user releases a key"},
                    {"onmousedown", {"*"},"Script to be run when a mouse button is pressed down on an element"},
                    {"onmousemove", {"*"},"Script to be run as long as the mouse pointer is moving over an element"},
                    {"onmouseout", {"*"},"Script to be run when a mouse pointer moves out of an element"},
                    {"onmouseover", {"*"},"Script to be run when a mouse pointer moves over an element"},
                    {"onmouseup", {"*"},"Script to be run when a mouse button is released over an element"},
                    {"onmousewheel", {"*"},"Script to be run when a mouse wheel is being scrolled over an element"},
                    {"onpaste", {"*"},"Script to be run when the user pastes some content in an element"},
                    {"onscroll", {"*"},"Script to be run when an element's scrollbar is being scrolled"},
                    {"onselect", {"*"},"Script to be run when the element gets selected"},
                    {"onwheel", {"*"},"Script to be run when the mouse wheel rolls up or down over an element"},
                    {"spellcheck", {"*"},"Specifies whether the element is to have its spelling and grammar checked or not"},
                    {"style", {"*"},"Specifies an inline CSS style for an element"},
                    {"tabindex", {"*"},"Specifies the tabbing order of an element"},
                    {"title", {"*"},"Specifies extra information about an element"},
                    {"translate", {"*"},"Specifies whether the content of an element should be translated or not"},
                    
                    {"accept", {"input"},"Specifies the types of files that the server accepts (only for type='file')"},
                    {"accept-charset", {"form"},"Specifies the character encodings that are to be used for the form submission"},
                    {"action", {"form"},"Specifies where to send the form-data when a form is submitted"},
                    {"alt", {"area","img","input"},"Specifies an alternate text when the original element fails to display"},
                    {"async", {"script"},"Specifies that the script is executed asynchronously (only for external scripts)"},
                    {"autocomplete", {"form","input"},"Specifies whether the <form> or the <input> element should have autocomplete enabled"},
                    {"autofocus", {"button","input","select","textarea"},"Specifies that the element should automatically get focus when the page loads"},
                    {"autoplay", {"audio","video"},"Specifies that the audio/video will start playing as soon as it is ready"},
                    {"charset", {"meta","script"},"Specifies the character encoding"},
                    {"checked", {"input"},"Specifies that an <input> element should be pre-selected when the page loads (for type='checkbox' or type='radio')"},
                    {"cite", {"blockquote","del","ins","q"},"Specifies a URL which explains the quote/deleted/inserted text"},
                    {"cols", {"textarea"},"Specifies the visible width of a text area"},
                    {"colspan", {"td","th"},"Specifies the number of columns a table cell should span"},
                    {"content", {"meta"},"Gives the value associated with the http-equiv or name attribute"},
                    {"controls", {"audio","video"},"Specifies that audio/video controls should be displayed (such as a play/pause button etc)"},
                    {"coords", {"area"},"Specifies the coordinates of the area"},
                    {"data", {"object"},"Specifies the URL of the resource to be used by the object"},
                    {"datetime", {"del","ins","time"},"Specifies the date and time"},
                    {"default", {"track"},"Specifies that the track is to be enabled if the user's preferences do not indicate that another track would be more appropriate"},
                    {"defer", {"script"},"Specifies that the script is executed when the page has finished parsing (only for external scripts)"},
                    {"dirname", {"input","textarea"},"Specifies that the text direction will be submitted"},
                    {"disabled", {"button","fieldset","input","optgroup","option","select","textarea"},"Specifies that the specified element/group of elements should be disabled"},
                    {"download", {"a","area"},"Specifies that the target will be downloaded when a user clicks on the hyperlink"},
                    {"enctype", {"form"},"Specifies how the form-data should be encoded when submitting it to the server (only for method='post')"},
                    {"for", {"label","output"},"Specifies which form element(s) a label/calculation is bound to"},
                    {"form", {"button","fieldset","input","label","meter","object","output","select","textarea"},"Specifies the name of the form the element belongs to"},
                    {"formaction", {"button","input"},"Specifies where to send the form-data when a form is submitted. Only for type='submit'"},
                    {"headers", {"td","th"},"Specifies one or more headers cells a cell is related to"},
                    {"height", {"canvas","embed","iframe","img","input","object","video"},"Specifies the height of the element"},
                    {"high", {"meter"},"Specifies the range that is considered to be a high value"},
                    {"href", {"a","area","base","link"},"Specifies the URL of the page the link goes to"},
                    {"hreflang", {"a","area","link"},"Specifies the language of the linked document"},
                    {"http-equiv", {"meta"},"Provides an HTTP header for the information/value of the content attribute"},
                    {"ismap", {"img"},"Specifies an image as a server-side image map"},
                    {"kind", {"track"},"Specifies the kind of text track"},
                    {"label", {"track","option","optgroup"},"Specifies the title of the text track"},
                    {"list", {"input"},"Refers to a <datalist> element that contains pre-defined options for an <input> element"},
                    {"loop", {"audio","video"},"Specifies that the audio/video will start over again, every time it is finished"},
                    {"low", {"meter"},"Specifies the range that is considered to be a low value"},
                    {"max", {"input","meter","progress"},"Specifies the maximum value"},
                    {"maxlength", {"input","textarea"},"Specifies the maximum number of characters allowed in an element"},
                    {"media", {"a","area","link","source","style"},"Specifies what media/device the linked document is optimized for"},
                    {"method", {"form"},"Specifies the HTTP method to use when sending form-data"},
                    {"min", {"input","meter"},"Specifies a minimum value"},
                    {"multiple", {"input","select"},"Specifies that a user can enter more than one value"},
                    {"muted", {"video","audio"},"Specifies that the audio output of the video should be muted"},
                    {"name", {"button","fieldset","form","iframe","input","map","meta","object","output","param","select","textarea"},"Specifies the name of the element"},
                    {"novalidate", {"form"},"Specifies that the form should not be validated when submitted"},
                    {"onabort", {"audio","embed","img","object","video"},"Script to be run on abort"},
                    {"onafterprint", {"body"},"Script to be run after the document is printed"},
                    {"onbeforeprint", {"body"},"Script to be run before the document is printed"},
                    {"onbeforeunload", {"body"},"Script to be run when the document is about to be unloaded"},
                    {"oncanplay", {"audio","embed","object","video"},"Script to be run when a file is ready to start playing (when it has buffered enough to begin)"},
                    {"oncanplaythrough", {"audio","video"},"Script to be run when a file can be played all the way to the end without pausing for buffering"},
                    {"oncuechange", {"track"},"Script to be run when the cue changes in a <track> element"},
                    {"ondurationchange", {"audio","video"},"Script to be run when the length of the media changes"},
                    {"onemptied", {"audio","video"},"Script to be run when something bad happens and the file is suddenly unavailable (like unexpectedly disconnects)"},
                    {"onended", {"audio","video"},"Script to be run when the media has reach the end (a useful event for messages like 'thanks for listening')"},
                    {"onerror", {"audio","body","embed","img","object","script","style","video"},"Script to be run when an error occurs"},
                    {"onhashchange", {"body"},"Script to be run when there has been changes to the anchor part of the a URL"},
                    {"onload", {"body","iframe","img","input","link","script","style"},"Script to be run when the element is finished loading"},
                    {"onloadeddata", {"audio","video"},"Script to be run when media data is loaded"},
                    {"onloadedmetadata", {"audio","video"},"Script to be run when meta data (like dimensions and duration) are loaded"},
                    {"onloadstart", {"audio","video"},"Script to be run just as the file begins to load before anything is actually loaded"},
                    {"onoffline", {"body"},"Script to be run when the browser starts to work offline"},
                    {"ononline", {"body"},"Script to be run when the browser starts to work online"},
                    {"onpagehide", {"body"},"Script to be run when a user navigates away from a page"},
                    {"onpageshow", {"body"},"Script to be run when a user navigates to a page"},
                    {"onpause", {"audio","video"},"Script to be run when the media is paused either by the user or programmatically"},
                    {"onplay", {"audio","video"},"Script to be run when the media has started playing"},
                    {"onplaying", {"audio","video"},"Script to be run when the media has started playing"},
                    {"onpopstate", {"body"},"Script to be run when the window's history changes."},
                    {"onprogress", {"audio","video"},"Script to be run when the browser is in the process of getting the media data"},
                    {"onratechange", {"audio","video"},"Script to be run each time the playback rate changes (like when a user switches to a slow motion or fast forward mode)."},
                    {"onreset", {"form"},"Script to be run when a reset button in a form is clicked."},
                    {"onresize", {"body"},"Script to be run when the browser window is being resized."},
                    {"onsearch", {"input"},"Script to be run when the user writes something in a search field (for <input='search'>)"},
                    {"onseeked", {"audio","video"},"Script to be run when the seeking attribute is set to false indicating that seeking has ended"},
                    {"onseeking", {"audio","video"},"Script to be run when the seeking attribute is set to true indicating that seeking is active"},
                    {"onstalled", {"audio","video"},"Script to be run when the browser is unable to fetch the media data for whatever reason"},
                    {"onstorage", {"body"},"Script to be run when a Web Storage area is updated"},
                    {"onsubmit", {"form"},"Script to be run when a form is submitted"},
                    {"onsuspend", {"audio","video"},"Script to be run when fetching the media data is stopped before it is completely loaded for whatever reason"},
                    {"ontimeupdate", {"audio","video"},"Script to be run when the playing position has changed (like when the user fast forwards to a different point in the media)"},
                    {"ontoggle", {"details"},"Script to be run when the user opens or closes the <details> element"},
                    {"onunload", {"body"},"Script to be run when a page has unloaded (or the browser window has been closed)"},
                    {"onvolumechange", {"audio","video"},"Script to be run each time the volume of a video/audio has been changed"},
                    {"onwaiting", {"audio","video"},"Script to be run when the media has paused but is expected to resume (like when the media pauses to buffer more data)"},
                    {"open", {"details"},"Specifies that the details should be visible (open) to the user"},
                    {"optimum", {"meter"},"Specifies what value is the optimal value for the gauge"},
                    {"pattern", {"input"},"Specifies a regular expression that an <input> element's value is checked against"},
                    {"placeholder", {"input","textarea"},"Specifies a short hint that describes the expected value of the element"},
                    {"poster", {"video"},"Specifies an image to be shown while the video is downloading, or until the user hits the play button"},
                    {"preload", {"audio","video"},"Specifies if and how the author thinks the audio/video should be loaded when the page loads"},
                    {"readonly", {"input","textarea"},"Specifies that the element is read-only"},
                    {"rel", {"a","area","form","link"},"Specifies the relationship between the current document and the linked document"},
                    {"required", {"input","select","textarea"},"Specifies that the element must be filled out before submitting the form"},
                    {"reversed", {"ol"},"Specifies that the list order should be descending (9,8,7...)"},
                    {"rows", {"textarea"},"Specifies the visible number of lines in a text area"},
                    {"rowspan", {"td","th"},"Specifies the number of rows a table cell should span"},
                    {"sandbox", {"iframe"},"Enables an extra set of restrictions for the content in an <iframe>"},
                    {"scope", {"th"},"Specifies whether a header cell is a header for a column, row, or group of columns or rows"},
                    {"selected", {"option"},"Specifies that an option should be pre-selected when the page loads"},
                    {"shape", {"area"},"Specifies the shape of the area"},
                    {"size", {"input","select"},"Specifies the width, in characters (for <input>) or specifies the number of visible options (for <select>)"},
                    {"sizes", {"img","link","source"},"Specifies the size of the linked resource"},
                    {"span", {"col","colgroup"},"Specifies the number of columns to span"},
                    {"src", {"audio","embed","iframe","img","input","script","source","track","video"},"Specifies the URL of the media file"},
                    {"srcdoc", {"iframe"},"Specifies the HTML content of the page to show in the <iframe>"},
                    {"srclang", {"track"},"Specifies the language of the track text data (required if kind='subtitles')"},
                    {"srcset", {"img","source"},"Specifies the URL of the image to use in different situations"},
                    {"start", {"ol"},"Specifies the start value of an ordered list"},
                    {"step", {"input"},"Specifies the legal number intervals for an input field"},
                    {"target", {"a","area","base","form"},"Specifies the target for where to open the linked document or where to submit the form"},
                    {"type", {"a","button","embed","input","link","menu","object","script","source","style"},"Specifies the type of element"},
                    {"usemap", {"img","object"},"Specifies an image as a client-side image map"},
                    {"value", {"button","input","li","option","meter","progress","param"},"Specifies the value of the element"},
                    {"width", {"canvas","embed","iframe","img","input","object","video"},"Specifies the width of the element"},
                    {"wrap", {"textarea"},"Specifies how the text in a text area is to be wrapped when submitted in a form"}
        };

		const attribute_info& get_attribute_info(const std::string& attrib_name)
		{
			for (const auto& ai : ATTRIBUTE_INFO)
				if (attrib_name == ai.name)
					return ai;

			static const attribute_info empty{ "", {}, ""};
			return empty;
		}

		const std::initializer_list<const char* const>& get_valid_tags_for_attribute(const std::string& attrib_name)
		{
			return get_attribute_info(attrib_name).valid_tags;
		}

		const char * const get_description_for_attribute(const std::string& attrib_name)
		{
			return get_attribute_info(attrib_name).description;
		}

		bool is_attribute_valid_for_tag(const std::string& tag_name, const std::string& attrib_name)
		{
			static const std::string wildcard("*");

			const auto& info = get_attribute_info(attrib_name);
			for (const auto& tag : info.valid_tags)
				if (wildcard == tag || tag_name == tag)
					return true;
			return false;
		}

	} // namespace htmldoc
} // namespace rda
