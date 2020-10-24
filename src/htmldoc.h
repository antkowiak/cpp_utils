#pragma once

//
// htmldoc.h - Utility to generate html pages.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2020-10-24
//

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace rda
{
	namespace htmldoc
	{
		class attribute
		{
		public:
			attribute(const std::string& key, const std::string& value) : m_key(key), m_value(value) {}
			virtual ~attribute() = default;
			std::string get_key() const { return m_key; }
			std::string get_value() const { return m_value; }
		protected:
			std::string m_key;
			std::string m_value;
		}; // class attribute

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
			virtual void add_attribute(const std::string& key, const std::string& value) { m_attributes.push_back(std::make_shared<attribute>(key, value)); }
			virtual void add_attribute(const std::string& key, const int value) { m_attributes.push_back(std::make_shared<attribute>(key, std::to_string(value))); }
			virtual void add_attribute(const std::string& key, const double value) { m_attributes.push_back(std::make_shared<attribute>(key, std::to_string(value))); }
			virtual void add_attribute(std::shared_ptr<attribute> pAttribute) { m_attributes.push_back(pAttribute); }
			virtual void add_style(const std::string& style) { m_styles.push_back(style); }
			virtual void add_child(std::shared_ptr<node> pNode) { m_content.clear(); if (pNode) m_children.push_back(pNode); }

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
					ss << m_content;
					ss << "</" << m_name << ">";
				}
				else if (!m_children.empty())
				{
					ss << ">";
					for (auto& child : m_children)
						if (child)
							ss << child->to_string();
					ss << "</" << m_name << ">";
				}
				else
					ss << " />";

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
					ss << indentStrContent << m_content << std::endl;
					ss << indentStr << "</" << m_name << ">";
				}
				else if (!m_children.empty())
				{
					ss << ">" << std::endl;;

					for (auto& child : m_children)
						if (child)
							ss << child->to_pretty_string(indent + 1);
					ss << indentStr << "</" << m_name << ">";
				}
				else
					ss << " />";

				ss << std::endl;
				return ss.str();
			}

		protected:
			std::string m_name;
			std::string m_content;
			std::vector<std::shared_ptr<attribute>> m_attributes;
			std::vector<std::string> m_styles;
			std::vector<std::shared_ptr<node>> m_children;
		}; // class node

		typedef std::shared_ptr<node> TNode;

#define NODE_FACTORY(NODE_NAME) \
    class node_##NODE_NAME : public node { public: node_##NODE_NAME() : node(#NODE_NAME) {} \
    static std::shared_ptr<node_##NODE_NAME> create() { return std::make_shared<node_##NODE_NAME>(); }};

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
			NODE_FACTORY(table)
			NODE_FACTORY(tbody)
			NODE_FACTORY(td)
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


			class document
		{
		public:
			document() {}
			document(const std::string& doctype) : m_doctype(doctype) {}
			document(const std::string& doctype, std::shared_ptr<node_html> root_node) : m_doctype(doctype), m_root_node(root_node) {}
			virtual ~document() = default;
			void set_doctype(const std::string& doctype) { m_doctype = doctype; }
			void set_root_node(std::shared_ptr<node_html> root_node) { m_root_node = root_node; }
			void print() const { std::cout << to_string(); }
			void pretty_print() const { std::cout << to_pretty_string(); }

			std::string to_string() const
			{
				std::stringstream ss;
				ss << "<!DOCTYPE " << m_doctype << ">" << std::endl;
				ss << m_root_node->to_string();
				return ss.str();
			}

			std::string to_pretty_string() const
			{
				std::stringstream ss;
				ss << "<!DOCTYPE " << m_doctype << ">" << std::endl;
				ss << m_root_node->to_pretty_string();
				return ss.str();
			}

		protected:
			std::string m_doctype = "html";
			std::shared_ptr<node_html> m_root_node;
		}; // class document
	} // namespace htmldoc
} // namespace rda
