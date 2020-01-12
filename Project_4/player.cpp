#include"player.h"
#include"actor.h"
#include"stack.h"
#include"list.h"
#include"point.h"
#include"aquarium.h"
#include<iostream>

//  Player( ... )
//		Constructs and initializes the Player/Actor and its member variables
//		Remembers and discovers the starting point.
Player::Player(Aquarium* aquarium, Point p, std::string name, char sprite)
	:Actor(aquarium, p, name, sprite),
	m_look(),
	m_discovered(),
	m_btQueue(),
	m_btStack(),
	m_toggleBackTracking(false) {

	// Discover the starting point
	m_discovered.push_front(p);
	m_look.push(p);
}

//  stuck() 
//		See if the player is stuck in the maze (no solution)
bool Player::stuck() const {
	return Actor::getState() == State::STUCK;
}

//  foundExit()
//		See if the player has found the exit
bool Player::foundExit() const {
	return Actor::getState() == State::FREEDOM;
}
//  toggleBackTrack( ... )
//		Turn on/off backtracking
void Player::toggleBackTrack(bool toggle) {
	m_toggleBackTracking = toggle;
}

//  getTargetPoint()
//		Get the point the player wants to look around next.
//      If m_look is empty then return an invalid point.
Point Player::getTargetPoint() const {
	if (m_look.empty()) return Point(-1, -1);
	return m_look.peek();
}

// discovered
//		returns true if the item is in the list
bool Player::discovered(const Point &p) const {
	return (m_discovered.find(p) != -1);
}



// say()
//		What does the player say?
//		Already Implemented, nothing to do here
void Player::say() {

	// Freedom supercedes being eaten
	if (getState() == State::FREEDOM) {
		std::cout << getName() << ": WEEEEEEEEE!";
		return;
	} 
 
	// Being eaten supercedes being lost
	switch (getInteract()) {
	case Interact::ATTACK:
		std::cout << getName() << ": OUCH!";
		break;
	case Interact::GREET:
		break;
	case Interact::ALONE:
	default:
		switch (getState()) {
		case State::LOOKING:
			std::cout << getName() << ": Where is the exit?";
			break;
		case State::STUCK:
			std::cout << getName() << ": Oh no! I am Trapped!";
			break;
		case State::BACKTRACK:
			std::cout << getName() << ": Got to backtrack...";
			break;
		default:
			break;
		}

		break;
	}
}


//  update() 
//		This function implements an algorithm to look through the maze
//      for places to move (STATE::LOOKING). update also handles moving the 
//		player as well, if there is an open undiscovered cell to move to the  
//		player just moves there.  However, there will be cases in the next 
//		desired point to look around is not adjacent (more than one cell away), 
//		at this point the player must backtrack (State::BACKTRACK) to a point 
//		that is adjacent to the next point the player was planning to visit
//
//		The player can only do one thing each call, they can either LOOK or 
//		BACKTRACK not both.  Nor should the player move more than one cell per
//		call to update.  If you examine the function calls for the entire game
//		you should observe that this is already be called within a loop. Your
//		Implementation should NOT have any loops that pertain to actual movement 
//		of the player.  
//
//		Backtracking is challenging, save it for the very very very last thing.
//		Make sure the STATE::LOOKING aspect compiles and works first.
void Player::update() {
    /* TODO */

    // Set by the settings file and checked here
    if(m_toggleBackTracking) {



        // if current position and next position is greater than 1, backtrack
        if (sqrDist(getPosition(), getTargetPoint()) > 1) {
            setState(State::BACKTRACK);
            m_btStack.pop();
            setPosition(m_btStack.peek());
            return;
        }
        else
            setState(State::LOOKING);
    }
    
    
    //Step 3. If stack is empty, no solution, we are stuck
    if (m_look.empty()){
        setState(State::STUCK);
        return;
    }
    
    m_btStack.push(m_look.peek());
    Point curr = m_look.peek();
    m_look.pop(); //Step 4.
    setPosition(curr);
    
    //Step 5. If we're at the endpoint, we're DONE
    if( curr == getAquarium() -> getEndPoint() ){
        setState(State::FREEDOM);
        return;
    }
    
    //Step 6. If slot to the west is opened and undiscovered, mark and push
    Point west = Point( curr.getX() - 1, curr.getY() );
    if ( getAquarium() -> isCellOpen(west) && !discovered(west) ){
        m_discovered.push_front(west); //mark as discovered
        m_look.push(west); //push on stack
    }

    //Step 7.
    Point east = Point( curr.getX() + 1, curr.getY() );
    if ( getAquarium() -> isCellOpen(east) && !discovered(east) ){
        m_discovered.push_front(east); //mark as discovered
        m_look.push(east); //push on stack
    }
    
    //Step 8.
    Point north = Point( curr.getX() , curr.getY() - 1 );
    if ( getAquarium() -> isCellOpen(north) && !discovered(north) ){
        m_discovered.push_front(north); //mark as discovered
        m_look.push(north); //push on stack
    }
    
    //Step 9.
    Point south = Point( curr.getX() , curr.getY() + 1 );
    if ( getAquarium() -> isCellOpen(south) && !discovered(south) ){
        m_discovered.push_front(south); //mark as discovered
        m_look.push(south); //push on stack
    }

}
