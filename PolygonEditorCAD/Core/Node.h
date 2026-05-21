#pragma once
#include "Setup.h"

class Node {
public:
	Node* parent_ = nullptr;
	vector <Node*> children_;//VERY IMPORTANT NOTE: children_ is not a list of dynamic objects but of adresses

	string name = "Node";
	bool collision = false;
	Vector2 position = { 0.0f, 0.0f };
	Vector2 size = { 1.0f, 1.0f };

	bool visible = true;

	virtual void process(double deltaTime) {}//should be defined in a seperate class

	void addChild(Node* child) {
		if (child == nullptr) return;
		child->parent_ = this;
		children_.push_back(child);
	}
	void removeChild(Node* child) {
		if (child == nullptr) return;

		auto it = remove(children_.begin(), children_.end(), child);

		if (it != children_.end()) {
			children_.erase(it, children_.end());
			child->parent_ = nullptr; // Object is now free/orphaned
		}
	}
	virtual void update(double deltaTime) {
		process(deltaTime);
		for (Node* child : children_) {
			child->update(deltaTime);
		}
	}
	virtual void draw() {
		if (!visible) return;
		for (Node* child : children_) {
			child->draw();
		}
	}

	Vector2 getGlobalPositon() const {
		if (parent_)
			return { parent_->getGlobalPositon() + position };
		return position;
	}
	void setSize(float x, float y) {
		size = { x,y };
	}
	Node* getParent() const { return parent_; }
	const auto& getChildren() const { return children_; }
	bool isRoot() const { return parent_ == nullptr; }
	string getName() { return name; }

};