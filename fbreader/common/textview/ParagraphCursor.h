/*
 * FBReader -- electronic book reader
 * Copyright (C) 2005 Nikolay Pultsin <geometer@mawhrin.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __PARAGRAPHCURSOR_H__
#define __PARAGRAPHCURSOR_H__

#include <vector>
#include <string>

#include <abstract/shared_ptr.h>

#include "../model/TextModel.h"

class TextElement;
class Paragraph;

class TextElementVector : public std::vector<TextElement*> {

public:
	~TextElementVector() VIEW_SECTION;
};

class WordCursor {

private:
	WordCursor() VIEW_SECTION;
	WordCursor(const TextElementVector::const_iterator &wordIterator) VIEW_SECTION;

public:
	~WordCursor() VIEW_SECTION;
	const WordCursor& operator = (const TextElementVector::const_iterator &wordIterator) VIEW_SECTION;
	bool sameElementAs(const WordCursor &cursor) const VIEW_SECTION;
	TextElement &element() const VIEW_SECTION;
	void nextWord() VIEW_SECTION;
	void previousWord() VIEW_SECTION;
	void setCharNumber(int num) VIEW_SECTION;
	int charNumber() const VIEW_SECTION;

private:
	TextElementVector::const_iterator myWordIterator;
	int myCharNumber;

friend class ParagraphCursor;
};

class ParagraphCursor {

public:
	static void clean() VIEW_SECTION;

private:
	static TextElement *ourHSpaceElement;
	static TextElement *ourBeforeParagraphElement;
	static TextElement *ourAfterParagraphElement;
	static TextElement *ourIndentElement;
	static TextElement *ourEmptyLineElement;

private:
	class ParagraphProcessor {

	public:
		ParagraphProcessor(const Paragraph &paragraph, const std::vector<TextMark> &marks, int paragraphNumber, const shared_ptr<TextElementVector> &elements) VIEW_SECTION;
		~ParagraphProcessor() VIEW_SECTION;

		void fill() VIEW_SECTION;

	private:
		void beforeAddWord() VIEW_SECTION;
		void addWord(const std::string &str, int start, int len) VIEW_SECTION;

	private:
		const Paragraph &myParagraph;
		shared_ptr<TextElementVector> myElements;

		std::vector<TextMark>::const_iterator myFirstMark;
		std::vector<TextMark>::const_iterator myLastMark;
		int myWordCounter;
		int myOffset;
	};
	friend class ParagraphProcessor;

protected:
	ParagraphCursor(const TextModel &model) VIEW_SECTION;
	ParagraphCursor(const ParagraphCursor &cursor) VIEW_SECTION;

public:
	static ParagraphCursor *createCursor(const TextModel &model) VIEW_SECTION;
	virtual ParagraphCursor *createCopy() const VIEW_SECTION = 0;
	virtual ~ParagraphCursor() VIEW_SECTION;

	virtual bool previous() VIEW_SECTION = 0;
	virtual bool next() VIEW_SECTION = 0;

	bool isStartOfText() const VIEW_SECTION;
	virtual bool isEndOfText() const VIEW_SECTION = 0;
	bool isStartOfParagraph() const VIEW_SECTION;
	bool isEndOfParagraph() const VIEW_SECTION;
	bool isEndOfSection() const VIEW_SECTION;

	int paragraphLength() const VIEW_SECTION;
	int paragraphNumber() const VIEW_SECTION;
	TextMark position() const VIEW_SECTION;
	int wordNumber(const WordCursor &wi) const VIEW_SECTION;
	int wordNumber() const VIEW_SECTION;
	int charNumber() const VIEW_SECTION;
	void moveTo(int paragraphNumber, int wordNumber, int charNumber) VIEW_SECTION;
	void moveToParagraphStart() VIEW_SECTION;
	void moveToParagraphEnd() VIEW_SECTION;

	WordCursor wordCursor() const VIEW_SECTION;
	void setWordCursor(const WordCursor cursor) VIEW_SECTION;
	const WordCursor begin() const VIEW_SECTION;
	const WordCursor end() const VIEW_SECTION;

private:
	void processControlParagraph(const Paragraph &paragraph) VIEW_SECTION;

protected:
	void fill() VIEW_SECTION;
	void clear() VIEW_SECTION;

private:
	/* assignment is disabled */
	ParagraphCursor &operator = (const ParagraphCursor &) VIEW_SECTION;
	
protected:
	const TextModel &myModel;
	std::vector<Paragraph*>::const_iterator myParagraphIterator;
	shared_ptr<TextElementVector> myElements;
	WordCursor myNextElement;
};

class PlainTextParagraphCursor : public ParagraphCursor {

private:
	PlainTextParagraphCursor(const PlainTextParagraphCursor &cursor) VIEW_SECTION;

public:
	PlainTextParagraphCursor(const TextModel &model) VIEW_SECTION;
	~PlainTextParagraphCursor() VIEW_SECTION;
	ParagraphCursor *createCopy() const VIEW_SECTION;

	bool previous() VIEW_SECTION;
	bool next() VIEW_SECTION;
	bool isEndOfText() const VIEW_SECTION;
};

class TreeParagraphCursor : public ParagraphCursor {

private:
	TreeParagraphCursor(const TreeParagraphCursor &cursor) VIEW_SECTION;

public:
	TreeParagraphCursor(const TreeModel &model) VIEW_SECTION;
	~TreeParagraphCursor() VIEW_SECTION;
	ParagraphCursor *createCopy() const VIEW_SECTION;

	bool previous() VIEW_SECTION;
	bool next() VIEW_SECTION;
	bool isEndOfText() const VIEW_SECTION;
};

inline WordCursor::WordCursor() { myCharNumber = 0; }
inline WordCursor::WordCursor(const TextElementVector::const_iterator &wordIterator) : myWordIterator(wordIterator) { myCharNumber = 0; }
inline WordCursor::~WordCursor() {}
inline const WordCursor& WordCursor::operator = (const TextElementVector::const_iterator &wordIterator) {
	myWordIterator = wordIterator;
	myCharNumber = 0;
	return *this;
}
inline bool WordCursor::sameElementAs(const WordCursor &cursor) const {
	return myWordIterator == cursor.myWordIterator;
}
inline TextElement &WordCursor::element() const { return **myWordIterator; }
inline void WordCursor::nextWord() { myWordIterator++; myCharNumber = 0; }
inline void WordCursor::previousWord() { myWordIterator--; myCharNumber = 0; }
inline void WordCursor::setCharNumber(int num) { myCharNumber = num; }
inline int WordCursor::charNumber() const { return myCharNumber; }

inline int ParagraphCursor::paragraphLength() const { return myElements->size(); }
inline int ParagraphCursor::paragraphNumber() const { return myParagraphIterator - myModel.paragraphs().begin(); }
inline int ParagraphCursor::wordNumber(const WordCursor &wi) const { return wi.myWordIterator - myElements->begin(); }
inline int ParagraphCursor::wordNumber() const { return wordNumber(myNextElement); }
inline int ParagraphCursor::charNumber() const { return myNextElement.myCharNumber; }
inline WordCursor ParagraphCursor::wordCursor() const { return myNextElement; }
inline void ParagraphCursor::setWordCursor(const WordCursor cursor) { myNextElement = cursor; }
inline const WordCursor ParagraphCursor::begin() const { return WordCursor(myElements->begin()); }
inline const WordCursor ParagraphCursor::end() const { return WordCursor(myElements->end()); }

inline PlainTextParagraphCursor::PlainTextParagraphCursor(const PlainTextParagraphCursor &cursor) : ParagraphCursor(cursor) {}
inline PlainTextParagraphCursor::PlainTextParagraphCursor(const TextModel &model) : ParagraphCursor(model) {}
inline PlainTextParagraphCursor::~PlainTextParagraphCursor() {}
inline ParagraphCursor *PlainTextParagraphCursor::createCopy() const { return new PlainTextParagraphCursor(*this); }

inline TreeParagraphCursor::TreeParagraphCursor(const TreeParagraphCursor &cursor) : ParagraphCursor(cursor) {}
inline TreeParagraphCursor::TreeParagraphCursor(const TreeModel &model) : ParagraphCursor(model) {}
inline TreeParagraphCursor::~TreeParagraphCursor() {}
inline ParagraphCursor *TreeParagraphCursor::createCopy() const { return new TreeParagraphCursor(*this); }

#endif /* __PARAGRAPHCURSOR_H__ */
